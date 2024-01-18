#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "tests/syscall_mock.h"

#include "builtin.h"
#include "execute.h"

/*** Needed local functions ***/

/* Parsing functions */
static bool is_cmd_exit(scommand cmd){
    assert(cmd != NULL);
    return(strcmp(scommand_front(cmd), "exit") == 0 
        || strcmp(scommand_front(cmd), "quit") == 0 
        || strcmp(scommand_front(cmd), "q") == 0 
        || strcmp(scommand_front(cmd), "salir") == 0);
}
/*
 * Indica si el comando cmd es el interno exit
 *
 * REQUIRES: cmd != NULL
 */

static bool is_cmd_cd(scommand cmd){
    assert(cmd != NULL);
    return(strcmp(scommand_front(cmd), "cd") == 0);
}
/*
 * Indica si el comando cmd es el interno cd
 *
 * REQUIRES: cmd != NULL
 */

static bool is_cmd_help(scommand cmd){
    assert(cmd != NULL);
    return(strcmp(scommand_front(cmd), "help") == 0);
}
/*
 * Indica si el comando cmd es el interno help
 *
 * REQUIRES: cmd != NULL
 */

/*
static bool is_cmd_change_color(scommand cmd){
    assert(cmd != NULL);
    return(strcmp(scommand_front(cmd), "color") == 0);
}

 * Indica si el comando cmd es el interno color
 *
 * REQUIRES: cmd != NULL
 */

/* Execution functions */
static void execute_exit(scommand cmd){
    assert(cmd != NULL && scommand_length(cmd) == 1);
    exit(EXIT_SUCCESS);
    // TODO: chequear que sea la manera correcta de salir
}
/*
 * Ejecuta el comando interno exit
 *
 * REQUIRES: cmd != NULL && scommand_length(cmd) == 1
 */

static void execute_cd(scommand cmd){
    assert(cmd != NULL);
    unsigned int argc = scommand_length(cmd);
    assert(argc >= 1);
    if(argc == 1){
        chdir(getenv("HOME"));
    }else{
        chdir(scommand_nth_arg(cmd,1u));
        // TODO: Chequear que sea la manera correcta de cambiar de directorio
    }
}

static void execute_help(scommand cmd){
    assert(cmd != NULL);
    if(scommand_length(cmd) == 1){
        printf("JASH(Just Another SHell)\n");
        printf("\nAuthors: (surname lexicographic order)\n");
        printf("\t - Baudino, Geremias\n");
        printf("\t - Guglieri, Juan Cruz\n");
        printf("\t - Mollea, Maria Agustina\n");
        printf("\n Internal commands:\n");
        printf("\t - cd [dir]: change current directory to dir. If dir is not specified, change to $HOME\n");
        printf("\t - exit/quit(q)/salir: exit the shell\n");
        printf("\t - help: show this help or man of the second argument\n");
        printf("\n");
    }else if(scommand_length(cmd) == 2){
        scommand aux = scommand_new();
        pipeline pipe = pipeline_new();
        scommand_push_back(aux, strdup("man"));
        scommand_push_back(aux, strdup(scommand_nth_arg(cmd, 1u)));
        pipeline_push_back(pipe, aux);
        execute_pipeline(pipe);
        pipeline_destroy(pipe);
    }
}

/*** Public functions ***/

bool builtin_is_internal(scommand cmd){
    assert(cmd != NULL);
    return(is_cmd_exit(cmd) || is_cmd_cd(cmd) || is_cmd_help(cmd));
}
/*
 * Indica si el comando alojado en `cmd` es un comando interno
 *
 * REQUIRES: cmd != NULL
 *
 */


bool builtin_alone(pipeline p){
    assert(p != NULL);
    return(pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p)));
}
/*
 * Indica si el pipeline tiene solo un elemento y si este se corresponde a un
 * comando interno.
 *
 * REQUIRES: p != NULL
 *
 * ENSURES:
 *
 * builtin_alone(p) == pipeline_length(p) == 1 &&
 *                     builtin_is_internal(pipeline_front(p))
 *
 *
 */

void builtin_run(scommand cmd){
    assert(builtin_is_internal(cmd));
    if(is_cmd_exit(cmd)){
        execute_exit(cmd);
    } else if(is_cmd_cd(cmd)){
        execute_cd(cmd);
    } else if(is_cmd_help(cmd)){
        execute_help(cmd);
    }
}
/*
 * Ejecuta un comando interno
 *
 * REQUIRES: {builtin_is_internal(cmd)}
 *
 */

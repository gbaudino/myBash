#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>		//constants and types
#include <sys/types.h>	//types (ex: pid_t)
#include <sys/wait.h>	//wait() waitpid()
#include <fcntl.h>		//open()

#include "tests/syscall_mock.h"

#include "command.h"
#include "parsing.h"
#include "builtin.h"
#include "execute.h"

#define READ_END 0 //Indice del extremo de lectura de un pipe
#define WRITE_END 1 //Indice del extremo de escritura de un pipe

/*	fd_t = "File descriptor type"
 *	De mucha utilidad a la hora de utilizar
 *	las funciones open() y dup2()
 */
typedef int fd_t;

void execute_pipeline(pipeline apipe){
    assert(apipe!=NULL);
    if (pipeline_is_empty(apipe)){
        return;
    }
    int status = 0;
    fd_t fd_in = STDIN_FILENO;
    fd_t fd_out = STDOUT_FILENO;
    uint pipe_length = pipeline_length(apipe); //Cantidad de comandos en la pipeline
    pid_t *pids = malloc(pipe_length*sizeof(pid_t)); //Arreglo de pids
    fd_t **pipes = NULL; //Array de pipes
    if (pipe_length > 1){
        pipes = (fd_t **)malloc((pipe_length - 1) * sizeof(fd_t *)); //Array de pipes necesarios
        for (uint i = 0; i < pipe_length - 1; i++){
            pipes[i] = (fd_t *)malloc(2 * sizeof(fd_t));
            status = pipe(pipes[i]);
            if(status == -1){
               fprintf(stderr, "ERROR: No fue posible crear el pipe.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    for (uint i = 0u; i < pipe_length; i++){
        scommand cmd = pipeline_nth_command(apipe, i);
        if (builtin_is_internal(cmd)){
            if(pipe_length > 1){
                fprintf(stderr, "ERROR: No es posbile ejecutar pipelines con comandos internos.\nEjecutando el comando interno.\n");
            }
            builtin_run(pipeline_nth_command(apipe, i));
            return;
        }
        pids[i] = fork();
        if (pids[i] == -1){
            fprintf(stderr, "ERROR: No fue posible crear el proceso hijo.\n");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0){
            //Proceso hijo
            uint j;
            for (j = 0u; j < pipe_length - 1u; j++){
                if (i - 1u != j){
                    close(pipes[j][READ_END]);
                }
                if (i != j){
                    close(pipes[j][WRITE_END]);
                }
            }
            
            if (i < (pipe_length - 1u)){
                //Si no es el ultimo comando
                if (scommand_get_redir_out(cmd) != NULL){
                    //Si hay redireccion de salida
                    fd_out = open(scommand_get_redir_out(cmd), O_WRONLY | O_CREAT | O_APPEND, S_IXUSR | S_IWUSR | S_IRUSR);
                    if (fd_out == -1){
                        fprintf(stderr, "ERROR: No fue posible abrir el archivo de salida.\n");
                        exit(EXIT_FAILURE);
                    }
                } else if (pipe_length > 1){
                    //Si no hay redireccion de salida y hay mas de un comando
                    fd_out = pipes[i][WRITE_END];
                }
                
            }
            if (i > 0){
                //Si no es el primer comando
                if (scommand_get_redir_in(cmd) != NULL){
                    //Si hay redireccion de entrada
                    fd_in = open(scommand_get_redir_in(cmd), O_RDONLY, S_IRUSR); //edite aca
                    if (fd_in == -1){
                        fprintf(stderr, "ERROR: No fue posible abrir el archivo de entrada.\n");
                        exit(EXIT_FAILURE);
                    }
                } else if (pipe_length > 1){
                    fd_in = pipes[i - 1][READ_END];
                }
            }
            if (i == 0){
                //Si es el primer comando
                if (scommand_get_redir_in(cmd) != NULL){
                    //Si hay redireccion de entrada
                    fd_in = open(scommand_get_redir_in(cmd), O_RDONLY, S_IRUSR); //edite aca
                    if (fd_in == -1){
                        fprintf(stderr, "ERROR: No fue posible abrir el archivo de entrada.\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            if (i == (pipe_length - 1)){
                //Si es el ultimo comando
                if (scommand_get_redir_out(cmd) != NULL){
                    //Si hay redireccion de salida
                    fd_out = open(scommand_get_redir_out(cmd), O_WRONLY | O_CREAT | O_APPEND, S_IXUSR | S_IWUSR | S_IRUSR); 
                    if (fd_out == -1){
                        fprintf(stderr, "ERROR: No fue posible abrir el archivo de salida.\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            if (fd_in != STDIN_FILENO){
                status = dup2(fd_in, STDIN_FILENO);
                if (status == -1){
                    fprintf(stderr, "ERROR: No fue posible duplicar el descriptor de archivo de entrada.\n");
                    exit(EXIT_FAILURE);
                }
                close(fd_in);
            }
            if (fd_out != STDOUT_FILENO){
                status = dup2(fd_out, STDOUT_FILENO);
                if (status == -1){
                    fprintf(stderr, "ERROR: No fue posible duplicar el descriptor de archivo de salida.\n");
                    exit(EXIT_FAILURE);
                }
                close(fd_out);
            }
            char ** args = scommand_to_argv(cmd);
            status = execvp(args[0], args);
            if (status == -1){
                fprintf(stderr, "ERROR: Ocurrio un error al ejecutar el comando.\n");
                exit(EXIT_FAILURE);
            }
            if (pipes != NULL){
                close (pipes[i - 1][READ_END]);
                close (pipes[i][WRITE_END]);
            }
            return;
        }
    }
    for (uint i = 0u; i < pipe_length - 1u; i++){
        close(pipes[i][READ_END]);
        close(pipes[i][WRITE_END]);
    }

    if (pipeline_get_wait(apipe)){
        for (uint i = 0; i < pipe_length; i++){
            waitpid(pids[i], NULL, 0);
        }
    }
    
    if (pipes != NULL){
        for (uint i = 0; i < pipe_length - 1; i++){
            free(pipes[i]);
        }
        free(pipes);
    }
    if (pids != NULL){
        free(pids);
    }
}

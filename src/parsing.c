#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "parser.h"
#include "parsing.h"

static bool check_null(scommand sc) {
    return (sc == NULL);
}
/*
 * Chequea que el comando sea valido
 */

static scommand parse_scommand(Parser p, char** error_msg) {
    assert(p != NULL);
    scommand sc = scommand_new();
    char* current_arg =
        NULL;  // Donde se guarda el argumento procesado por el parser
    arg_kind_t* arg_type =
        malloc(sizeof(arg_kind_t));  // Nos indica el tipo del argumento
                                     // (NORMAL,INPUT u OUTPUT)
    bool read_continue =
        true;  // Si aun hay argumentos por leer y no hubo un error

    parser_skip_blanks(p);
    current_arg = parser_next_argument(p, arg_type);
    if (current_arg == NULL) {
        read_continue = false;
        sc = scommand_destroy(sc);
        sc = NULL;
    } else if (*arg_type == ARG_NORMAL) {
        scommand_push_back(sc, current_arg);
        while (!parser_at_eof(p) && read_continue) {
            parser_skip_blanks(p);
            current_arg = parser_next_argument(p, arg_type);
            if (*arg_type == ARG_NORMAL && current_arg != NULL) {
                scommand_push_back(
                    sc, current_arg);  // Esto si lo que proceso coincide con un
                                       // nombre de un comando o argumento
            } else if (*arg_type == ARG_INPUT) {
                if (current_arg == NULL) {
                    read_continue = false;
                    sc = scommand_destroy(sc);
                    sc = NULL;
                    *error_msg =
                        strdup("No se encontro un archivo de entrada.");
                } else {
                    scommand_set_redir_in(sc, current_arg);
                }
            } else if (*arg_type == ARG_OUTPUT) {
                if (current_arg == NULL) {
                    read_continue = false;
                    sc = scommand_destroy(sc);
                    sc = NULL;
                    *error_msg = strdup("No se encontro un archivo de salida.");
                } else {
                    scommand_set_redir_out(sc, current_arg);
                }
            } else {
                read_continue = false;
            }
        }
    } else {
        sc = scommand_destroy(sc);
        sc = NULL;
        *error_msg = strdup(
            "No se ha encontrado un comando previo al redireccionamiento.");
    }

    free(current_arg);
    free(arg_type);
    current_arg = NULL;
    arg_type = NULL;

    return sc;
}
/*
 * Parsea un comando de la linea de comando y lo
 * devuelve en una estructura scommand
 * Si no hay comando o hay un error devuelve NULL
 * y en error_msg se guarda el mensaje de error
 *
 * Require: p != NULL
 */

pipeline parse_pipeline(Parser p) {
    pipeline result = pipeline_new();
    scommand cmd = NULL;
    bool error = false, another_pipe = true;
    char* error_msg = NULL;

    cmd = parse_scommand(p, &error_msg);
    error = check_null(cmd);

    while (another_pipe && !error) {
        pipeline_push_back(result, cmd);

        parser_op_pipe(p, &another_pipe);
        if (another_pipe) {
            cmd = parse_scommand(p, &error_msg);
            error = check_null(cmd);
            if (error) {
                free(error_msg);
                error_msg = strdup(
                    "Secuencia de comandos incompleta. La secuencia de "
                    "comandos no puede terminar con un pipe.");
            }
        }
        parser_skip_blanks(p);
    }

    parser_skip_blanks(p);

    bool wait_for_child = true;
    parser_op_background(p, &wait_for_child);
    if (result != NULL) {
        pipeline_set_wait(result, !wait_for_child);
    }

    parser_skip_blanks(p);

    bool* garbage = malloc(sizeof(bool));
    parser_garbage(p, garbage);
    if (*garbage && !error) {
        error_msg = strdup("Se encontraron caracteres luego de finalizar la secuencia de comandos.");
        error = true;
    }
    free(garbage);
    garbage = NULL;

    if (error) {
        if (error_msg != NULL) {
            fprintf(stderr, "ERROR: %s\n", error_msg);
            free(error_msg);
        }
        if (result != NULL) {
            result = pipeline_destroy(result);
            result = NULL;
        }
    }

    return result;
}
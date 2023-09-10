#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "strextra.h"

/**** Needed local functions ****/

static GSList* list_tail(GSList* list, GDestroyNotify free_function) {
    assert(list != NULL && free_function != NULL);
    gpointer front = g_slist_nth_data(list, 0);
    GSList* result = g_slist_remove(list, front);
    free_function(front);
    return (result);
}
/*
 * Elimina el primer elemento de la lista y libera la memoria
 * asociada a él, devolviendo la lista sin el primer elemento.
 * Requires: list != NULL && free_function != NULL
 *
 */

static void free_scommand(void* self) {
    assert(self != NULL);
    scommand aux = self;
    scommand_destroy(aux);
}
/*
 * Libera la memoria asociada a un scommand.
 * Es necesaria para usarse como parámetro de funcion
 * de liberacion de memoria de una lista de scommands.
 * Requires: self != NULL
 *
 */

static char* concate_scmd_to_string(char* str, const scommand self) {
    assert(str != NULL && self != NULL);

    char* scommand_string = scommand_to_string(self);
    str = strconcate(str, scommand_string);
    free(scommand_string);
    scommand_string = NULL;

    assert(scommand_string == NULL);
    return (str);
}
/*
 * Concatena el string de un scommand a otro string.
 * Devuelve el string concatenado.
 * Libera la memoria del string del scommand.
 * Requires: str != NULL && self != NULL
 *
 */

/**** TAD comando simple ****/

struct scommand_s {
    GSList* args;
    char* in;
    char* out;
};

scommand scommand_new(void) {
    scommand result = malloc(sizeof(struct scommand_s));

    result->args = NULL;
    result->in = NULL;
    result->out = NULL;

    assert(result != NULL && scommand_is_empty(result) &&
           scommand_get_redir_in(result) == NULL &&
           scommand_get_redir_out(result) == NULL);

    return result;
}

scommand scommand_destroy(scommand self) {
    assert(self != NULL);
    if (self->args != NULL) {
        g_slist_free_full(self->args, free);
        self->args = NULL;
    }
    if (self->in != NULL) {
        free(self->in);
        self->in = NULL;
    }
    if (self->out != NULL) {
        free(self->out);
        self->out = NULL;
    }
    if (self != NULL) {
        free(self);
        self = NULL;
    }

    assert(self == NULL);
    return self;
}

/* Modificadores */

void scommand_push_back(scommand self, char* argument) {
    assert(self != NULL && argument != NULL);
    self->args = g_slist_append(self->args, argument);
    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self) {
    assert(self != NULL && !scommand_is_empty(self));
    self->args = list_tail(self->args, free);
}

void scommand_set_redir_in(scommand self, char* filename) {
    assert(self != NULL);
    self->in = filename;
}

void scommand_set_redir_out(scommand self, char* filename) {
    assert(self != NULL);
    self->out = filename;
}

/* Proyectores */

bool scommand_is_empty(const scommand self) {
    assert(self != NULL);
    return (self->args == NULL);
}

unsigned int scommand_length(const scommand self) {
    assert(self != NULL);
    unsigned int length = g_slist_length(self->args);
    assert((length == 0) == scommand_is_empty(self));
    return (length);
}

char* scommand_front(const scommand self) {
    assert(self != NULL && !scommand_is_empty(self));
    char* result = NULL;
    result = g_slist_nth_data(self->args, 0);
    assert(result != NULL);
    return (result);
}

char* scommand_get_redir_in(const scommand self) {
    assert(self != NULL);
    return self->in;
}

char* scommand_get_redir_out(const scommand self) {
    assert(self != NULL);
    return self->out;
}

char* scommand_to_string(const scommand self) {
    assert(self != NULL);

    char* result = strdup("");
    GSList* auxargs = self->args;

    while (auxargs != NULL) {
        result = strconcate(result, g_slist_nth_data(auxargs, 0));
        auxargs = g_slist_next(auxargs);
        if (auxargs != NULL) {
            result = strconcate(result, " ");
        }
    }

    if (self->in != NULL) {
        result = strconcate(result, " < ");
        result = strconcate(result, self->in);
    }
    if (self->out != NULL) {
        result = strconcate(result, " > ");
        result = strconcate(result, self->out);
    }

    assert(scommand_is_empty(self) || scommand_get_redir_in(self) == NULL ||
           scommand_get_redir_out(self) == NULL || strlen(result) > 0);
    return result;
}

/* Funciones auxiliares */

char* scommand_nth_arg(const scommand self, unsigned int n) {
    assert(self != NULL && n < scommand_length(self));
    char* result = NULL;
    result = g_slist_nth_data(self->args, n);
    assert(result != NULL);
    return (result);
}

char** scommand_to_argv(const scommand self) {
    assert(self != NULL);
    char** result = NULL;
    char* current = NULL;
    unsigned int length = scommand_length(self);
    if (length > 0) {
        result = malloc(sizeof(char*) * (length + 1));
        for (unsigned int i = 0; i < length; i++) {
            current = scommand_nth_arg(self, i);
            result[i] = strdup(current);
        }
        result[length] = NULL;
    } else {
        result = malloc(sizeof(char*));
        result[0] = NULL;
    }
    assert(result != NULL);
    return result;
}

/* TAD comando pipeado */

struct pipeline_s {
    GSList* commands;
    bool needWait;
};

pipeline pipeline_new(void) {
    pipeline result = malloc(sizeof(struct pipeline_s));
    assert(result != NULL);
    result->commands = NULL;
    result->needWait = true;
    assert(result != NULL && pipeline_is_empty(result) &&
           pipeline_get_wait(result));
    return result;
}

pipeline pipeline_destroy(pipeline self) {
    assert(self != NULL);
    if (!pipeline_is_empty(self)) {
        g_slist_free_full(self->commands, free_scommand);
        self->commands = NULL;
    }
    if (self != NULL) {
        free(self);
        self = NULL;
    }
    assert(self == NULL);
    return self;
}

/* Modificadores */

void pipeline_push_back(pipeline self, scommand sc) {
    assert(self != NULL && sc != NULL);
    self->commands = g_slist_append(self->commands, sc);
    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self) {
    assert(self != NULL && !pipeline_is_empty(self));
    self->commands = list_tail(self->commands, free_scommand);
}

void pipeline_set_wait(pipeline self, const bool w) {
    assert(self != NULL);
    self->needWait = w;
}

/* Proyectores */

bool pipeline_is_empty(const pipeline self) {
    assert(self != NULL);
    return (self->commands == NULL);
}

unsigned int pipeline_length(const pipeline self) {
    assert(self != NULL);
    unsigned int length = g_slist_length(self->commands);
    assert((length == 0) == pipeline_is_empty(self));
    return (length);
}

scommand pipeline_front(const pipeline self) {
    assert(self != NULL && !pipeline_is_empty(self));
    scommand result = NULL;
    result = g_slist_nth_data(self->commands, 0);
    assert(result != NULL);
    return (result);
}

bool pipeline_get_wait(const pipeline self) {
    assert(self != NULL);
    return (self->needWait);
}

char* pipeline_to_string(const pipeline self) {
    assert(self != NULL);

    GSList* auxcmds = self->commands;
    char* result = strdup("");

    while (auxcmds != NULL) {
        result = concate_scmd_to_string(result, g_slist_nth_data(auxcmds, 0));
        auxcmds = g_slist_next(auxcmds);
        if (auxcmds != NULL) {
            result = strconcate(result, " | ");
        }
    }

    if (!pipeline_get_wait(self)) {
        result = strconcate(result, " &");
    }

    assert(pipeline_is_empty(self) || pipeline_get_wait(self) ||
           strlen(result) > 0);

    return result;
}

/* Funciones auxiliares */

scommand pipeline_nth_command(const pipeline self, unsigned int n) {
    assert(self != NULL && n < pipeline_length(self));
    scommand result = NULL;
    result = g_slist_nth_data(self->commands, n);
    assert(result != NULL);
    return (result);
}

/* End of file */
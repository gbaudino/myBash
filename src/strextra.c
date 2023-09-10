#include <assert.h>   /* assert()...                        */
#include <stdlib.h>   /* calloc()...                        */
#include <string.h>   /* strlen(), strncat, strcopy()...    */
#include "strextra.h" /* Interfaz                           */

char* strconcate(char* s1, char* s2) {
    char* concate = NULL;
    size_t len_s1 = strlen(s1);
    size_t len_s2 = strlen(s2);
    assert(s1 != NULL && s2 != NULL);
    concate = calloc(len_s1 + len_s2 + 1, sizeof(char));
    strncpy(concate, s1, len_s1);
    concate = strncat(concate, s2, len_s2);
    assert(concate != NULL && strlen(concate) == strlen(s1) + strlen(s2));
    free(s1);
    s1 = NULL;
    return concate;
}
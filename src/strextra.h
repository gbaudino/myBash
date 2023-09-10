#ifndef _STREXTRA_H_
#define _STREXTRA_H_


/*
 * Concatena las cadenas en s1 y s2 devolviendo nueva memoria (debe ser
 * liberada por el llamador con free())
 *
 * USAGE:
 *
 * concate = strconcate(s1, s2);
 *
 * REQUIRES:
 *     s1 != NULL &&  s2 != NULL
 *
 * ENSURES:
 *     concate != NULL && strlen(concate) == strlen(s1) + strlen(s2)
 *     s1 == NULL
 *
 */
char* strconcate(char* s1, char* s2);


#endif

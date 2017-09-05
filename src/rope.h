/* Heap based implementation of the rope structure. */

#ifndef ROPE_H
#define ROPE_H

#include "bintree.h"

typedef BinaryTree Rope;

/* Creates a new empty Rope.
 *
 * On success, a pointer to the newly created Rope is returned. On error the
 * program terminates abnormally. */
Rope *Rope_new();

/* Creates a new Rope containing the string text.
 *
 * On success, a pointer to the newly created Rope is returned. On error the
 * program terminates abnormally.
 *
 * The string pointed to by text is duplicated in memory by this function.
 *
 * If text is a null pointer, the behaviour of the program is implementation
 * dependent. */
Rope *Rope_newFrom(const char *text);

void Rope_delete(Rope *self);

Rope *Rope_insert(Rope *self, int pos, const char *text);

/* Given a position p, separate self in two.
 *
 * On success, a pointer to the right side rope is returned. On error,
 * NULL is returned.
 *
 * self is modified in place.
 *
 * Rope_split splits the rope after p characters counting from the left.
 * Thus, spliting with p == 1 will split after the first character,
 * and splitting with p == 0 will create an empty rope to the left and move
 * the entire string to the right.
 *
 * A negative position will be considered an offset relative to the end of
 * the rope. So position -1 is the last character of the rope. */
Rope *Rope_split(Rope *self, int p); /* Stub. Always returns NULL. */

/* Concatenates l_rope and r_rope. */
Rope *Rope_join(Rope *l_rope, Rope *r_rope);

int Rope_size(const Rope *self);

/* Returns the contents of the Rope as a null-terminated string.
 *
 * This function returns a pointer to a new string which holds the full
 * contents of self. Memory for the new string is obtained with malloc,
 * and can be freed with free. */
char *Rope_toString(const Rope *self);

#endif

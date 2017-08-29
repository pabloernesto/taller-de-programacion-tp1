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
Rope *Rope_newFrom(char *text);

void Rope_delete(Rope *self);

/* Given a position p, separate self in two.
 *
 * On success, a pointer to the right side rope is returned. On error,
 * NULL is returned.
 *
 * self is modified in place.
 *
 * Rope_split splits the rope from the character at offset p to the right.
 * If for example position is 0, the result will be a one character rope
 * to the left, and the rest of the rope to the right.
 *
 * A negative position will be considered an offset relative to the end of
 * the rope. So position -1 is the last character of the rope. */
Rope *Rope_split(Rope *self, int p); /* Stub. Always returns NULL. */

/* Concatenates l_rope and r_rope. */
Rope *Rope_join(Rope *l_rope, Rope *r_rope);

int Rope_size(const Rope *self);

#endif

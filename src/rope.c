/* Heap based implementation of the rope structure. */

#define _POSIX_C_SOURCE 201708L

#include "rope.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int value;
    char *text;
} RopeContent;

static void deleteContent(void *content);
static Rope *splitRecursive(Rope *self, int p);
static Rope *splitLeaf(Rope *self, int p);
static int getValue(const Rope *self);
static void setValue(Rope *self, int value);
static char *getText(const Rope *self);
static void toStringRecurse(const Rope *self, char *s);

Rope *Rope_new() {
    return Rope_newFrom("");
}

Rope *Rope_newFrom(const char *text) {
    RopeContent *c = (RopeContent *) malloc(sizeof(RopeContent));
    assert(c != NULL);

    *c = (RopeContent) { .value = strlen(text), .text = strdup(text) };
    assert(c->text != NULL);

    return BinaryTree_new(c, NULL, NULL);
}

void Rope_destroy(Rope *self) {
    BinaryTree_delete(self, deleteContent);
}

Rope *Rope_insert(Rope *self, int pos, const char *text) {
    if (pos < 0) pos += Rope_size(self) + 1;
    assert(pos >= 0);

    Rope *right = Rope_split(self, pos);
    return Rope_join(self, Rope_join(Rope_newFrom(text), right));
}

Rope *Rope_delete(Rope *self, int begin, int end) {
    if (begin < 0) begin += Rope_size(self) + 1;
    if (end < 0) end += Rope_size(self) + 1;
    assert((begin >= 0) && (end >= 0));

    Rope *last = Rope_split(self, end);
    Rope *middle = Rope_split(self, begin);

    Rope_destroy(middle);

    return Rope_join(self, last);
}

Rope *Rope_split(Rope *self, int p) {
    if (p < 0) p += Rope_size(self) + 1;
    assert(p >= 0);

    return splitRecursive(self, p);
}

Rope *Rope_join(Rope *l_rope, Rope *r_rope) {
    /* If either rope is empty, delete it and return the other. */
    if (0 == getValue(l_rope)) {
        Rope_destroy(l_rope);
        return r_rope;
    }

    if (0 == getValue(r_rope)) {
        Rope_destroy(r_rope);
        return l_rope;
    }

    /* Otherwise, join them with a new root. */
    Rope *root = Rope_new();

    setValue(root, Rope_size(l_rope));
    BinaryTree_insertLeft(root, l_rope);

    BinaryTree_insertRight(root, r_rope);

    return root;
}

int Rope_size(const Rope *self) {
    /* If self is null. */
    if (self == NULL) return 0;

    /* If this is a leaf. */
    if (BinaryTree_isLeaf(self)) {
        /* If this is an empty rope. */
        if (getText(self) == NULL) return 0;

        return strlen(getText(self));
    }

    return getValue(self) + Rope_size(BinaryTree_rchild(self));
}

char *Rope_toString(const Rope *self) {
    int size = Rope_size(self) + 1;
    char *s = (char *) malloc(size);
    assert(s != NULL);

    toStringRecurse(self, s);
    s[size - 1] = '\0';
    return s;
}

static void deleteContent(void *self) {
    RopeContent *cast = (RopeContent *) self;
    if (cast->text != NULL) free(cast->text);
    free(cast);
}

static Rope *splitRecursive(Rope *self, int p) {
    if (BinaryTree_isLeaf(self)) return splitLeaf(self, p);

    if (p < getValue(self)) {
        Rope *rchild = BinaryTree_extractRight(self);

        int original_value = getValue(self);
        Rope *left_result = splitRecursive(BinaryTree_lchild(self), p);
        setValue(self, original_value - Rope_size(left_result));
        return Rope_join(left_result, rchild);
    } else if (p > getValue(self)) {
        return splitRecursive(BinaryTree_rchild(self), p - getValue(self));
    }

    return BinaryTree_extractRight(self);
}

static Rope *splitLeaf(Rope *self, int p) {
    assert(p >= 0);

    char *text = getText(self);
    int len = strlen(text);
    assert(p <= len);

    Rope *ret = Rope_newFrom(text + p);
    text[p] = '\0';
    setValue(self, strlen(text));
    return ret;
}

static int getValue(const Rope *self) {
    RopeContent *c = (RopeContent *) BinaryTree_getLiveContent(self);
    return c->value;
}

static void setValue(Rope *self, int value) {
    RopeContent *c =(RopeContent *) BinaryTree_getLiveContent(self);
    c->value = value;
}

static char *getText(const Rope *self) {
    RopeContent *c = (RopeContent *) BinaryTree_getLiveContent(self);
    return c->text;
}

static void toStringRecurse(const Rope *self, char *s) {
    /* NULL pointer. */
    if (self == NULL) return;

    /* Leaf. */
    if (BinaryTree_isLeaf(self)) {
        /* Empty leaf. */
        if (getText(self) == NULL) return;

        /* Non empty leaf. */
        strcpy(s, getText(self));
        return;
    }

    /* Recurse. */
    toStringRecurse(BinaryTree_lchild(self), s);
    toStringRecurse(BinaryTree_rchild(self), s + getValue(self));
}

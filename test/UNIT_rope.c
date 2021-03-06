/* Battery of unit tests for the project's rope implementation. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/rope.h"

static void test_sizeOfEmptyStringIsZero();
static void test_sizeLeaf();
static void test_sizeSmallFullTree();
static void test_sizeSmallPartialTree();

static void test_toStringOfEmptyRopeReturnsEmptyString();
static void test_toStringOfLeaf();
static void test_toStringOfSmallFullTree();
static void test_toStringOfSmallPartialTree();

static void test_splitAtOffset0LeafOnly();
static void test_splitAtOffset1LeafOnly();
static void test_splitAtEndIsNoopLeafOnly();
static void test_splitLastCharLeafOnly();

static void test_splitSmallFullTreeAtJoin();
static void test_splitSmallFullTree();
static void test_splitAt0LargePartialTree();
static void test_splitLargePartialTree();
static void test_splitLargePartialTreeAndLeaf();

static void test_insertAt0InEmptyRope();
static void test_insertAtEndInEmptyRope();
static void test_insertAt0InLargePartialTree();
static void test_insertAtEndInLargePartialTree();
static void test_insertInTheMiddleInLargePartialTree();

static void test_deleteBeginingLeafOnly();
static void test_deleteEndLeafOnly();

static void test_growTreeFromEmptyRope();

int main(int argc, char **argv) {
    test_sizeOfEmptyStringIsZero();
    test_sizeLeaf();
    test_sizeSmallFullTree();
    test_sizeSmallPartialTree();

    test_toStringOfEmptyRopeReturnsEmptyString();
    test_toStringOfLeaf();
    test_toStringOfSmallFullTree();
    test_toStringOfSmallPartialTree();

    test_splitAtOffset0LeafOnly();
    test_splitAtOffset1LeafOnly();
    test_splitAtEndIsNoopLeafOnly();
    test_splitLastCharLeafOnly();

    test_splitSmallFullTreeAtJoin();
    test_splitSmallFullTree();
    test_splitAt0LargePartialTree();
    test_splitLargePartialTree();
    test_splitLargePartialTreeAndLeaf();

    test_insertAt0InEmptyRope();
    test_insertAtEndInEmptyRope();
    test_insertAt0InLargePartialTree();
    test_insertAtEndInLargePartialTree();
    test_insertInTheMiddleInLargePartialTree();

    test_deleteBeginingLeafOnly();
    test_deleteEndLeafOnly();

    test_growTreeFromEmptyRope();

    printf("All tests ok.\n");
}

static void test_sizeOfEmptyStringIsZero() {
    Rope *r = Rope_new();
    assert(Rope_size(r) == 0);

    Rope_destroy(r);
}

static void test_sizeLeaf() {
    Rope *r = Rope_newFrom("123456");
    assert(Rope_size(r) == 6);

    Rope_destroy(r);
}

static void test_sizeSmallFullTree() {
    Rope *left = Rope_newFrom("12345");
    Rope *right = Rope_newFrom("67890");
    Rope *r = Rope_join(left, right);
    assert(Rope_size(r) == 10);

    Rope_destroy(r);
}

static void test_sizeSmallPartialTree() {
    Rope *t1 = Rope_newFrom("123");
    Rope *t2 = Rope_newFrom("456");
    Rope *t3 = Rope_newFrom("789");
    Rope *r = Rope_join(Rope_join(t1, t2), t3);
    assert(Rope_size(r) == 9);

    Rope_destroy(r);
}

static void test_toStringOfEmptyRopeReturnsEmptyString() {
    Rope *r = Rope_new();
    char *s = Rope_toString(r);
    assert(strcmp("", s) == 0);

    Rope_destroy(r);
    free(s);
}

static void test_toStringOfLeaf() {
    Rope *r = Rope_newFrom("123456");
    char *s = Rope_toString(r);
    assert(strcmp(s, "123456") == 0);

    Rope_destroy(r);
    free(s);
}

static void test_toStringOfSmallFullTree() {
    Rope *left = Rope_newFrom("12345");
    Rope *right = Rope_newFrom("67890");
    Rope *r = Rope_join(left, right);
    char *s = Rope_toString(r);
    assert(strcmp(s, "1234567890") == 0);

    Rope_destroy(r);
    free(s);
}

static void test_toStringOfSmallPartialTree() {
    Rope *t1 = Rope_newFrom("123");
    Rope *t2 = Rope_newFrom("456");
    Rope *t3 = Rope_newFrom("789");
    Rope *r = Rope_join(Rope_join(t1, t2), t3);
    char *s = Rope_toString(r);
    assert(strcmp(s, "123456789") == 0);

    Rope_destroy(r);
    free(s);
}

static void test_splitAtOffset0LeafOnly() {
    Rope *a = Rope_newFrom("Hello World!");
    Rope *b = Rope_split(a, 0);

    assert(Rope_size(a) == 0);
    char *s = Rope_toString(a);
    assert(strcmp("", s) == 0);
    free(s);

    assert(Rope_size(b) == 12);
    s = Rope_toString(b);
    assert(strcmp("Hello World!", s) == 0);
    free(s);

    Rope_destroy(a);
    Rope_destroy(b);
}

static void test_splitAtOffset1LeafOnly() {
    Rope *a = Rope_newFrom("Hello World!");
    Rope *b = Rope_split(a, 1);

    assert(Rope_size(a) == 1);
    char *s = Rope_toString(a);
    assert(strcmp("H", s) == 0);
    free(s);

    assert(Rope_size(b) == 11);
    s = Rope_toString(b);
    assert(strcmp("ello World!", s) == 0);
    free(s);

    Rope_destroy(a);
    Rope_destroy(b);
}

static void test_splitAtEndIsNoopLeafOnly() {
    Rope *a = Rope_newFrom("Hello World!");
    Rope *b = Rope_split(a, -1);

    assert(Rope_size(a) == 12);
    char *s = Rope_toString(a);
    assert(strcmp("Hello World!", s) == 0);
    free(s);

    assert(Rope_size(b) == 0);
    s = Rope_toString(b);
    assert(strcmp("", s) == 0);
    free(s);

    Rope_destroy(a);
    Rope_destroy(b);
}

static void test_splitLastCharLeafOnly() {
    Rope *a = Rope_newFrom("Hello World!");
    Rope *b = Rope_split(a, -2);

    assert(Rope_size(a) == 11);
    char *s = Rope_toString(a);
    assert(strcmp("Hello World", s) == 0);
    free(s);

    assert(Rope_size(b) == 1);
    s = Rope_toString(b);
    assert(strcmp("!", s) == 0);
    free(s);

    Rope_destroy(a);
    Rope_destroy(b);
}

static void test_splitSmallFullTreeAtJoin() {
    Rope *left = Rope_join(Rope_newFrom("12345"), Rope_newFrom("67890"));
    Rope *right = Rope_split(left, 5);

    assert(Rope_size(left) == 5);
    char *s = Rope_toString(left);
    assert(strcmp(s, "12345") == 0);
    free(s);

    assert(Rope_size(right) == 5);
    s = Rope_toString(right);
    assert(strcmp(s, "67890") == 0);
    free(s);

    Rope_destroy(left);
    Rope_destroy(right);
}

static void test_splitSmallFullTree() {
    Rope *left = Rope_join(Rope_newFrom("12345"), Rope_newFrom("67890"));
    Rope *right = Rope_split(left, 1);

    char *s = Rope_toString(left);
    assert(strcmp(s, "1") == 0);
    free(s);

    s = Rope_toString(right);
    assert(strcmp(s, "234567890") == 0);
    free(s);

    Rope_destroy(left);
    Rope_destroy(right);
}

static void test_splitAt0LargePartialTree() {
    Rope *a = Rope_join(Rope_join(Rope_newFrom("To "), Rope_newFrom("be ")),
                        Rope_join(Rope_newFrom("or "), Rope_newFrom("not?")));
    Rope *b = Rope_split(a, 0);

    assert(Rope_size(a) == 0);
    char *s = Rope_toString(a);
    assert(strcmp(s, "") == 0);
    free(s);

    assert(Rope_size(b) == 13);
    s = Rope_toString(b);
    assert(strcmp(s, "To be or not?") == 0);
    free(s);

    Rope_destroy(a);
    Rope_destroy(b);
}

static void test_splitLargePartialTree() {
    Rope *a = Rope_join(Rope_join(Rope_newFrom("To "), Rope_newFrom("be ")),
                        Rope_join(Rope_newFrom("or "), Rope_newFrom("not?")));
    Rope *b = Rope_split(a, 3);

    char *s = Rope_toString(a);
    assert(strcmp(s, "To ") == 0);
    free(s);

    s = Rope_toString(b);
    assert(strcmp(s, "be or not?") == 0);
    free(s);

    Rope_destroy(a);
    Rope_destroy(b);
}

static void test_splitLargePartialTreeAndLeaf() {
    Rope *a = Rope_join(Rope_join(Rope_newFrom("To "), Rope_newFrom("be ")),
                        Rope_join(Rope_newFrom("or "), Rope_newFrom("not?")));
    Rope *b = Rope_split(a, 7);

    char *s = Rope_toString(a);
    assert(strcmp(s, "To be o") == 0);
    free(s);

    s = Rope_toString(b);
    assert(strcmp(s, "r not?") == 0);
    free(s);

    Rope_destroy(a);
    Rope_destroy(b);
}

static void test_insertAt0InEmptyRope() {
    Rope *r = Rope_new();
    r = Rope_insert(r, 0, "Hello");

    char *s = Rope_toString(r);
    assert(strcmp(s, "Hello") == 0);
    free(s);

    Rope_destroy(r);
}

static void test_insertAtEndInEmptyRope() {
    Rope *r = Rope_new();
    r = Rope_insert(r, -1, "Hello");

    char *s = Rope_toString(r);
    assert(strcmp(s, "Hello") == 0);
    free(s);

    Rope_destroy(r);
}

static void test_insertAt0InLargePartialTree() {
    Rope *r = Rope_join(Rope_join(Rope_newFrom("o "), Rope_newFrom("be ")),
                        Rope_join(Rope_newFrom("or "), Rope_newFrom("not?")));
    r = Rope_insert(r, 0, "T");

    char *s = Rope_toString(r);
    assert(strcmp(s, "To be or not?") == 0);
    free(s);

    Rope_destroy(r);
}

static void test_insertAtEndInLargePartialTree() {
    Rope *r = Rope_join(Rope_join(Rope_newFrom("To "), Rope_newFrom("be ")),
                        Rope_join(Rope_newFrom("or "), Rope_newFrom("not")));
    r = Rope_insert(r, -1, "?");

    char *s = Rope_toString(r);
    assert(strcmp(s, "To be or not?") == 0);
    free(s);

    Rope_destroy(r);
}

static void test_insertInTheMiddleInLargePartialTree() {
    Rope *r = Rope_join(Rope_join(Rope_newFrom("To"), Rope_newFrom("be ")),
                        Rope_join(Rope_newFrom("or "), Rope_newFrom("not?")));
    r = Rope_insert(r, 2, " ");

    char *s = Rope_toString(r);
    assert(strcmp(s, "To be or not?") == 0);
    free(s);

    Rope_destroy(r);
}

static void test_deleteBeginingLeafOnly() {
    Rope *r = Rope_newFrom("Hello World!");
    r = Rope_delete(r, 0, 6);

    assert(Rope_size(r) == 6);
    char *s = Rope_toString(r);
    assert(strcmp("World!", s) == 0);
    free(s);

    Rope_destroy(r);
}

static void test_deleteEndLeafOnly() {
    Rope *r = Rope_newFrom("Hello World!");
    r = Rope_delete(r, 5, -1);

    assert(Rope_size(r) == 5);
    char *s = Rope_toString(r);
    assert(strcmp("Hello", s) == 0);
    free(s);

    Rope_destroy(r);
}

static void test_growTreeFromEmptyRope() {
    Rope *r = Rope_new();
    r = Rope_insert(r, -1, "Hello");
    r = Rope_insert(r, -1, "!");

    assert(Rope_size(r) == 6);
    char *s = Rope_toString(r);
    assert(strcmp("Hello!", s) == 0);
    free(s);

    Rope_destroy(r);
}

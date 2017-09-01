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

static void test_splitAtOffset1LeafOnly();
static void test_splitAtEndIsNoopLeafOnly();
static void test_splitLastCharLeafOnly();

static void test_splitSmallFullTreeAtJoin();
static void test_splitSmallFullTree();

int main(int argc, char **argv) {
    test_sizeOfEmptyStringIsZero();
    test_sizeLeaf();
    test_sizeSmallFullTree();
    test_sizeSmallPartialTree();

    test_toStringOfEmptyRopeReturnsEmptyString();
    test_toStringOfLeaf();
    test_toStringOfSmallFullTree();
    test_toStringOfSmallPartialTree();

    test_splitAtOffset1LeafOnly();
    test_splitAtEndIsNoopLeafOnly();
    test_splitLastCharLeafOnly();

    test_splitSmallFullTreeAtJoin();
    test_splitSmallFullTree();

    printf("All tests ok.\n");
}

static void test_sizeOfEmptyStringIsZero() {
    Rope *r = Rope_new();
    assert(Rope_size(r) == 0);

    Rope_delete(r);
}

static void test_sizeLeaf() {
    Rope *r = Rope_newFrom("123456");
    assert(Rope_size(r) == 6);

    Rope_delete(r);
}

static void test_sizeSmallFullTree() {
    Rope *left = Rope_newFrom("12345");
    Rope *right = Rope_newFrom("67890");
    Rope *r = Rope_join(left, right);
    assert(Rope_size(r) == 10);

    Rope_delete(r);
}

static void test_sizeSmallPartialTree() {
    Rope *t1 = Rope_newFrom("123");
    Rope *t2 = Rope_newFrom("456");
    Rope *t3 = Rope_newFrom("789");
    Rope *r = Rope_join(Rope_join(t1, t2), t3);
    assert(Rope_size(r) == 9);

    Rope_delete(r);
}

static void test_toStringOfEmptyRopeReturnsEmptyString() {
    Rope *r = Rope_new();
    char *s = Rope_toString(r);
    assert(strcmp("", s) == 0);

    Rope_delete(r);
    free(s);
}

static void test_toStringOfLeaf() {
    Rope *r = Rope_newFrom("123456");
    char *s = Rope_toString(r);
    assert(strcmp(s, "123456") == 0);

    Rope_delete(r);
    free(s);
}

static void test_toStringOfSmallFullTree() {
    Rope *left = Rope_newFrom("12345");
    Rope *right = Rope_newFrom("67890");
    Rope *r = Rope_join(left, right);
    char *s = Rope_toString(r);
    assert(strcmp(s, "1234567890") == 0);

    Rope_delete(r);
    free(s);
}

static void test_toStringOfSmallPartialTree() {
    Rope *t1 = Rope_newFrom("123");
    Rope *t2 = Rope_newFrom("456");
    Rope *t3 = Rope_newFrom("789");
    Rope *r = Rope_join(Rope_join(t1, t2), t3);
    char *s = Rope_toString(r);
    assert(strcmp(s, "123456789") == 0);

    Rope_delete(r);
    free(s);
}

static void test_splitAtOffset1LeafOnly() {
    Rope *a = Rope_newFrom("Hello World!");
    Rope *b = Rope_split(a, 1);

    char *s = Rope_toString(a);
    assert(strcmp("H", s) == 0);
    free(s);

    s = Rope_toString(b);
    assert(strcmp("ello World!", s) == 0);
    free(s);

    Rope_delete(a);
    Rope_delete(b);
}

static void test_splitAtEndIsNoopLeafOnly() {
    Rope *a = Rope_newFrom("Hello World!");
    Rope *b = Rope_split(a, -1);

    char *s = Rope_toString(a);
    assert(strcmp("Hello World!", s) == 0);
    free(s);

    s = Rope_toString(b);
    assert(strcmp("", s) == 0);
    free(s);

    Rope_delete(a);
    Rope_delete(b);
}

static void test_splitLastCharLeafOnly() {
    Rope *a = Rope_newFrom("Hello World!");
    Rope *b = Rope_split(a, -2);

    char *s = Rope_toString(a);
    assert(strcmp("Hello World", s) == 0);
    free(s);

    s = Rope_toString(b);
    assert(strcmp("!", s) == 0);
    free(s);

    Rope_delete(a);
    Rope_delete(b);
}

static void test_splitSmallFullTreeAtJoin() {
    Rope *left = Rope_join(Rope_newFrom("12345"), Rope_newFrom("67890"));
    Rope *right = Rope_split(left, 5);

    char *s = Rope_toString(left);
    assert(strcmp(s, "12345") == 0);
    free(s);

    s = Rope_toString(right);
    assert(strcmp(s, "67890") == 0);
    free(s);

    Rope_delete(left);
    Rope_delete(right);
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

    Rope_delete(left);
    Rope_delete(right);
}

/* Battery of unit tests for the project's rope implementation. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/rope.h"

static void test_sizeOfEmptyStringIsZero();
static void test_sizeLeaf();
static void test_toStringOfEmptyRopeReturnsEmptyString();
static void test_splitAtOffset0LeafOnly();
static void test_splitAtEndIsNoopLeafOnly();
static void test_splitLastCharLeafOnly();

int main(int argc, char **argv) {
    test_sizeOfEmptyStringIsZero();
    test_sizeLeaf();
    test_toStringOfEmptyRopeReturnsEmptyString();
    test_splitAtOffset0LeafOnly();
    test_splitAtEndIsNoopLeafOnly();
    test_splitLastCharLeafOnly();
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

static void test_toStringOfEmptyRopeReturnsEmptyString() {
    Rope *r = Rope_new();
    char *s = Rope_toString(r);
    assert(strcmp("", s) == 0);

    Rope_delete(r);
    free(s);
}

static void test_splitAtOffset0LeafOnly() {
    Rope *a = Rope_newFrom("Hello World!");
    Rope *b = Rope_split(a, 0);

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

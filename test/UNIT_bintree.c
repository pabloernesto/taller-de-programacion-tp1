/* Battery of unit tests for the project's binary tree implementation. */

#include <stdio.h>
#include <assert.h>
#include "../src/bintree.h"

static void noop(void *object);
static void test_errorAlreadyHaslchild();
static void test_addToRoot();

int main(int argc, char **argv) {
    test_errorAlreadyHaslchild();
    test_addToRoot();
    printf("All tests ok.\n");
}

static void test_errorAlreadyHaslchild() {
    BinaryTree *tree = BinaryTree_new(NULL, NULL, NULL);

    int return_value = BinaryTree_newLeft(tree, NULL);
    assert(return_value == 0);

    return_value = BinaryTree_newLeft(tree, NULL);
    assert(return_value == -1);

    BinaryTree_delete(tree, noop);
}

static void test_addToRoot() {
    char a = 'a';
    char b = 'b';

    BinaryTree *tree = BinaryTree_new(&a, NULL, NULL);
    tree = BinaryTree_new(&b, tree, NULL);

    assert('b' == *((char*) BinaryTree_getContent(tree)));
    assert('a' == *((char*) BinaryTree_getContent(BinaryTree_lchild(tree))));

    BinaryTree_delete(tree, noop);
}

static void noop(void *object) {}

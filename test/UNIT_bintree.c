/* Battery of unit tests for the project's binary tree implementation. */

#include <stdio.h>
#include <assert.h>
#include "../src/bintree.h"

int tests_run = 0;

static void noop(void *object);
static void test_errorAlreadyHaslchild();

int main(int argc, char **argv) {
    test_errorAlreadyHaslchild();
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

static void noop(void *object) {}

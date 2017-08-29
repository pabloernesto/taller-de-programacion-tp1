#include <stdio.h>
#include "bintree.h"

int main(int argc, char **argv) {
    char *string1 = "Hello";
    char *string2 = "World!";

    BinaryTree *tree = BinaryTree_new(string1, NULL, NULL);
    BinaryTree_newRight(tree, string2);

    /*        Hello
     *      /      \
     * NULL         World!
     *
     */

    BinaryTree *root = tree;
    BinaryTree *child = BinaryTree_rchild(tree);

    printf("%s %s\n", (char *) BinaryTree_getLiveContent(root),
                      (char *) BinaryTree_getLiveContent(child));

    BinaryTree_delete(tree, noOP);
}

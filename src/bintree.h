/* Heap based implementation of the binary tree structure. */

#ifndef BINTREE_H
#define BINTREE_H

typedef struct BinaryTreeNode BinaryTree;

typedef void (*Deallocator)(void *object);

void NOOP(void *object) {}

BinaryTree *BinaryTree_new(void *content, BinaryTree *lchild,
                           BinaryTree *rchild);

/* Creates a new node around content, and places it under self.
 *
 * On success, zero is returned. On error, -1 is returned. */
int BinaryTree_newLeft(BinaryTree *self, void *content);
int BinaryTree_newRight(BinaryTree *self, void *content);

void BinaryTree_deleteLeft(BinaryTree *self, Deallocator f);
void BinaryTree_deleteRight(BinaryTree *self, Deallocator f);
void BinaryTree_delete(BinaryTree *self, Deallocator f);

/* Gets the content of the current position of the tree.
 *
 * This function returns the contents of the tree node, not a copy;
 * modify at your own risk. */
void *BinaryTree_getContent(const BinaryTree *self);

BinaryTree *BinaryTree_lchild(const BinaryTree *self);
BinaryTree *BinaryTree_rchild(const BinaryTree *self);

/* Places package as subtree of target.
 *
 * On success, zero is returned. On error, -1 is returned. */
int BinaryTree_insertLeft(BinaryTree *target, BinaryTree *package);
int BinaryTree_insertRight(BinaryTree *target, BinaryTree *package);

#endif

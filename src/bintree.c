/* Heap based implementation of the binary tree structure. */

#include "bintree.h"
#include <stdlib.h>
#include <assert.h>

struct BinaryTreeNode {
    struct BinaryTreeNode *lchild, *rchild;
    void *content;
};

void noOP(void *object) {}

BinaryTree *BinaryTree_new(void *content, BinaryTree *lchild,
                           BinaryTree *rchild) {
    BinaryTree *self = (BinaryTree *) malloc(sizeof(BinaryTree));
    assert(self != NULL);

    *self = (BinaryTree) {
        .content = content,
        .lchild = lchild,
        .rchild = rchild
    };
    return self;
}

void BinaryTree_deleteLeft(BinaryTree *self, Deallocator f) {
    if (self->lchild != NULL) BinaryTree_delete(self->lchild, f);
    self->lchild = NULL;
}

void BinaryTree_deleteRight(BinaryTree *self, Deallocator f) {
    if (self->rchild != NULL) BinaryTree_delete(self->rchild, f);
    self->rchild = NULL;
}

void BinaryTree_delete(BinaryTree *self, Deallocator f) {
    if (self == NULL) return;

    BinaryTree_deleteLeft(self, f);
    BinaryTree_deleteRight(self, f);
    f(self->content);
    free(self);
}

void *BinaryTree_getLiveContent(const BinaryTree *self) {
    return self->content;
}

BinaryTree *BinaryTree_lchild(const BinaryTree *self) {
    return self->lchild;
}

BinaryTree *BinaryTree_rchild(const BinaryTree *self) {
    return self->rchild;
}

int BinaryTree_isLeaf(const BinaryTree *self) {
    return (NULL == BinaryTree_lchild(self)) &&
           (NULL == BinaryTree_rchild(self));
}

int BinaryTree_insertLeft(BinaryTree *target, BinaryTree *package) {
    if (target->lchild != NULL) return -1;
    target->lchild = package;
    return 0;
}

int BinaryTree_insertRight(BinaryTree *target, BinaryTree *package) {
    if (target->rchild != NULL) return -1;
    target->rchild = package;
    return 0;
}

BinaryTree *BinaryTree_extractLeft(BinaryTree *self) {
    BinaryTree *x = self->lchild;
    self->lchild = NULL;
    return x;
}

BinaryTree *BinaryTree_extractRight(BinaryTree *self) {
    BinaryTree *x = self->rchild;
    self->rchild = NULL;
    return x;
}

int BinaryTree_newLeft(BinaryTree *self, void *content) {
    if (self->lchild != NULL) return -1;
    self->lchild = BinaryTree_new(content, NULL, NULL);
    return 0;
}

int BinaryTree_newRight(BinaryTree *self, void *content) {
    if (self->rchild != NULL) return -1;
    self->rchild = BinaryTree_new(content, NULL, NULL);
    return 0;
}

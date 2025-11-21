#ifndef BIN_TREE_H
#define BIN_TREE_H

#include "structs.h"

TreeErr_t TreeVerify(Tree *tree);
Tree*     TreeInit(size_t size, Node *root);
Node*     NodeInit(TreeElem value, Node *left, Node *right);
TreeErr_t TreeInsertNode(Tree *tree, TreeElem value);
TreeErr_t TreeDestroy(Tree *tree);
TreeErr_t NodeDestroy(Node *node);
int       PrintTree(const Node *node, char *buffer);

#endif // BIN_TREE_H
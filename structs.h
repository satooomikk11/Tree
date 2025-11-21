#ifndef STRUCTS_H
#define STRUCTS_H

#include <math.h>

const int BUFFER_CONST = 1024;

typedef char* TreeElem;

struct Node
{
    TreeElem value;     // данные узла
    struct Node *left;  // левый  потомок
    struct Node *right; // правый потомок
};

struct Tree
{
    Node *root;         // корень дерева
    size_t size;        // кол-во узлов
};

enum TreeErr_t
{
    TREE_OK = 0,
    TREE_ERR_INSERT,
    TREE_ERR_DELETE,
    TREE_ERR_MEMORY,
    TREE_ERR_PTR,
    TREE_ERR_FILE,
    TREE_INVALID_PTR
};

#endif // STRUCTS_H
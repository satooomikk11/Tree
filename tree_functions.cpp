#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <math.h>
#include <string.h>

#include "structs.h"
#include "tree_functions.h"
#include "tree_dump.h"


// date → [banana → apple, cherry]
// ( ( apple ) banana ( cherry ) ) date
int PrintTree(const Node* node, char* buffer)
{
    assert(node);
    assert(buffer);

    int pos = 0;
    pos += sprintf(buffer + pos, "(");
    if (node->left) 
    {
        pos += PrintTree(node->left, buffer + pos);
    }
    pos += sprintf(buffer + pos, "%s ", node->value);
    
    if (node->right) 
    {
        pos += PrintTree(node->right, buffer + pos);
    }
    pos += sprintf(buffer + pos, ")");
    
    return pos;
}

// создание узла дерева с возможными потомками
Node* NodeInit(TreeElem value, Node* left, Node* right)
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    if (!node)
    {
        printf("Calloc не сработал :(\n");
        return NULL;
    }
    node->left  = left;
    node->right = right;
    node->value = value;

    return node;
}

// создание дерева (передаём размер и корень)
Tree* TreeInit(size_t size, Node* root)
{
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    if (!tree)
    {
        printf("Calloc  не сработал :(\n");
        return NULL;
    }

    tree->root = root;
    tree->size = (root != NULL) ? size : 0;

    return tree;
}

TreeErr_t NodeDestroy(Node* node)
{
    if (!node)
    {
        return TREE_OK;
    }

    NodeDestroy(node->left);
    NodeDestroy(node->right);
    if (node->value != NULL)
    {
        free(node->value);
    }
    free(node);

    return TREE_OK;
}

TreeErr_t TreeDestroy(Tree* tree)
{
    if (!tree)
    {
        return TREE_OK;
    }
    
    NodeDestroy(tree->root);
    free(tree);
    
    return TREE_OK;
}

// вставка узла
TreeErr_t TreeInsertNode(Tree* tree, TreeElem value)
{
    assert(tree);
    TreeDump(tree, "tree.dot");

    // создаем новый узел
    Node* node = NodeInit(value, NULL, NULL);
    // если дерево пустое, то новый узел - его корень
    if (!tree->root)
    {
        tree->root = node;
        tree->size = 1;
        return TREE_OK;
    }

    Node* current = tree->root;
    Node* parent  = NULL;

    // спускаемся с корня
    while (current != NULL)
    {
        parent = current;
        // сравниваем значение с предыдущим
        if (strcmp(value, current->value) <= 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    // вставка узла
    if (strcmp(value, parent->value) <= 0)
    {
        parent->left  = node;
    }
    else
    {
        parent->right = node;
    }

    tree->size++;
    TreeDump(tree, "tree.dot");

    return TREE_OK;
}

TreeErr_t TreeVerify(Tree* tree)
{
    if (!tree)
    {
        return TREE_ERR_PTR;
    }
    if (tree->root && (tree->size == 0))
    {
        return TREE_INVALID_PTR;
    }
    
    return TREE_OK;
}
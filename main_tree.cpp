#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <math.h>
#include <string.h>

#include "structs.h"
#include "tree_functions.h"
#include "tree_dump.h"
#include "akinator.h"

/*
Tree* CreateTree()
{
    Node* list1 = NodeInit(strdup("Янковский"),       NULL, NULL);
    Node* list2 = NodeInit(strdup("Павел Прилучный"), NULL, NULL);
    Node* list3 = NodeInit(strdup("Джонни Депп"),     NULL, NULL);
    Node* list4 = NodeInit(strdup("Бурунов"),         NULL, NULL);

    Node* node3 = NodeInit(strdup("русский?"), list3, list2);
    Node* node2 = NodeInit(strdup("молодой?"), node3, list1);
    Node* node1 = NodeInit(strdup("сейчас на хайпе?"), list1, node2);

    Node* root = node1;

    return TreeInit(7, root); 
}
*/

int main()
{
    Tree* tree;
    TreeLoadFromFile(tree, "tree_data.txt"); 
    // Tree* tree = CreateTree();
    TreeDump(tree, "tree.dot");

    Akinator(tree);
    TreeSaveToFile(tree, "tree_data.txt");
    
    // TreeDestroy(tree);

    return 0;
}
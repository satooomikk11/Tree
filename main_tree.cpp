#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "structs.h"
#include "tree_functions.h"
#include "tree_dump.h"

int main()
{ 
    // вот тут мы просто создаём листы (т.е крайние элементы)
    Node* leaf1 = NodeInit(strdup("apple"),  NULL, NULL);
    Node* leaf2 = NodeInit(strdup("cherry"), NULL, NULL);
    Node* leaf3 = NodeInit(strdup("fig"),    NULL, NULL);
    Node* leaf4 = NodeInit(strdup("kiwi"),   NULL, NULL);
    
    // тут создаём две ветки и указываем, какие листы на какой ветке
    Node* node1 = NodeInit(strdup("banana"),     leaf1, leaf2); // левая  ветка
    Node* node2 = NodeInit(strdup("elderberry"), leaf3, leaf4); // правая ветка
    
    // создаем корень и указываем его ветке
    Node* root = NodeInit(strdup("date"), node1, node2);
    
    Tree* tree = TreeInit(7, root); // 7 узлов всего
    
    // а вот тут уже вставляем узлы в ветку в левую ветку (к banana)
    TreeInsertNode(tree, strdup("apricot"));   // должен пойти влево  от banana
    TreeInsertNode(tree, strdup("blueberry")); // должен пойти вправо от banana
    
    // добавляем в правую ветку (к elderberry)  
    TreeInsertNode(tree, strdup("grape"));    // должен пойти влево  от elderberry
    TreeInsertNode(tree, strdup("honeydew")); // должен пойти вправо от elderberry
    
    // добавляем узлы, которые создадут новые уровни
    TreeInsertNode(tree, strdup("avocado"));  // пойдет глубоко влево
    TreeInsertNode(tree, strdup("zucchini")); // пойдет глубоко вправо
    
    char buffer[1024];
    PrintTree(tree->root, buffer);
    
    TreeDestroy(tree);

    return 0;
}
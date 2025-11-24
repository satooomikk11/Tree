#ifndef AKINATOR_H
#define AKINATOR_H

#include "structs.h"

TreeErr_t Akinator(Tree* tree);
TreeErr_t AkinatorCreateObject(Tree* tree, Node* current_node);
TreeErr_t AkinatorFindNode(Tree* tree, Node* current_node);

TreeErr_t TreeSaveToFile(Tree* tree, const char* filename);
TreeErr_t TreeSaveNodeToFile(Node* node, FILE* filestream);

void SkipSpace(const char** str_ptr);
size_t CountNodes(Node* node);

TreeErr_t TreeLoadFromFile(Tree* tree, const char* filename);
TreeErr_t CreateTreeFromStr(Tree* tree, const char* str);
Node* CreateNodeFromStr(const char** str_ptr);

#endif //AKINATOR_H
#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include <stdio.h>

#include "structs.h"

TreeErr_t TreeDump(Tree *tree, const char *filename);
TreeErr_t DotGenerateTreeHeader(Tree *tree, FILE *file);
TreeErr_t DotGenerateTreeNode(Node *node, FILE *file);
TreeErr_t MakeTreeHTML(const Tree *tree, FILE *html_file, long timestamp, long microseconds);

#endif // TREE_DUMP_H
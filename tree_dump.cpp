#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "structs.h"
#include "tree_functions.h"
#include "tree_dump.h"

TreeErr_t TreeDump(Tree *tree, const char *filename)
{
    assert(tree);

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        return TREE_ERR_FILE;
    }

    DotGenerateTreeHeader(tree, file);
    DotGenerateTreeNode(tree->root, file);
    fprintf(file, "}");

    fclose(file);

    char *buffer = (char *)calloc(BUFFER_CONST, sizeof(char));
    if (!buffer)
    {
        printf("Calloc не сработал :(\n");
        return TREE_ERR_MEMORY;
    }

    // используем микросекунды для уникальности имени файла
    struct timeval tv = {0, 0};
    gettimeofday(&tv, NULL);
    long timestamp    = tv.tv_sec;
    long microseconds = tv.tv_usec;
    
    sprintf(buffer, "dot -Tpng %s -o png/tree_dump%ld_%ld.png", filename, timestamp, microseconds);

    int status = system(buffer);
    if (status != 0)
    {
        free(buffer);
        return TREE_ERR_FILE;
    }
    
    free(buffer);

    FILE *html_file = fopen("dump.html", "a");
    if (html_file)
    {
        MakeTreeHTML(tree, html_file, timestamp, microseconds); 
        fclose(html_file);
    }

    return TREE_OK;
}

// создаём заголовок .dot файла для дерева
TreeErr_t DotGenerateTreeHeader(Tree *tree, FILE *file)
{
    assert(tree);
    assert(file);

    fprintf(file, "digraph Tree\n{\n\n");

    return TREE_OK;
}

// генерируем узлы дерева
TreeErr_t DotGenerateTreeNode(Node *node, FILE *file)
{
    assert(node);
    assert(file);

    fprintf(file, "\tnode%p[shape=Mrecord, style=\"rounded, filled\", fillcolor=\"lightblue\", "
                  "label=\"{%p | {%s} | {%p | %p}}\"]\n", 
                  (void*)node, (void*)node, node->value, (void*)node->left, (void*)node->right);

    if (node->left)
    {
        fprintf(file, "\tnode%p -> node%p\n", (void*)node, (void*)node->left);
        DotGenerateTreeNode(node->left, file);
    }

    if (node->right)
    {
        fprintf(file, "\tnode%p -> node%p\n", (void*)node, (void*)node->right);
        DotGenerateTreeNode(node->right, file);
    }

    return TREE_OK;
}

// запись dump-а дерева в html-файл
TreeErr_t MakeTreeHTML(const Tree *tree, FILE *html_file, long timestamp, long microseconds)
{
    assert(tree);
    assert(html_file);
    
    // получаем дату и время из timestamp
    struct tm *timeinfo;
    char time_buffer[80];
    timeinfo = localtime(&timestamp);

    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    fprintf(html_file, "<pre>\n<hr>\n    <font size=\"10\"> ");
    fprintf(html_file, "TREE DUMP - %s.%06ld (timestamp: %ld)\n", time_buffer, microseconds, timestamp);

    fprintf(html_file, "    Sort Tree : ");
    char *buffer = (char *)calloc(BUFFER_CONST, sizeof(char));
    if (buffer)
    {
        PrintTree(tree->root, buffer);
        fprintf(html_file, "%s\n", buffer);
        fprintf(html_file, "\n");
        free(buffer);
    }
    
    fprintf(html_file, "<img src=\"png/tree_dump%ld_%ld.png\" width=\"800\">\n\n", timestamp, microseconds);

    return TREE_OK;
}
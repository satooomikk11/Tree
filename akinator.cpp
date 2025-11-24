#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <math.h>
#include <string.h>

#include "structs.h"
#include "tree_functions.h"
#include "tree_dump.h"
#include "akinator.h"

const int SHORT_BUFFER_CONST = 10;
const int LONG_BUFFER_CONST  = 1024;

// запрашивает у пользователя ответы и пытается угадать объект
TreeErr_t Akinator(Tree* tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        Node* root = NodeInit(strdup("неизвестно что"), NULL, NULL);
        tree = TreeInit(1, root);
    }
    
    char play_again[SHORT_BUFFER_CONST] = {};

    while (strcmp(play_again, "STOP") != 0)
    {
        printf("Загадай актёра, а я постараюсь его отгадать!\nОтвечай на вопросы YES или NO.\n");
        
        AkinatorFindNode(tree, tree->root);

        printf("\nСыграем ещё? (YES/STOP): ");
        scanf("%5s", play_again);
        getchar();
        
    } 

    printf("Спасибо за игру! Возвращайся!\n");
    
    return TREE_OK;
}

// рекурсивно проходит по дереву, задавая вопросы и определяя направление движения (влево/вправо)
TreeErr_t AkinatorFindNode(Tree* tree, Node* current_node) 
{
    assert(current_node);
    assert(tree);

    char user_answer[SHORT_BUFFER_CONST] = {};    

    if (current_node->left == NULL && current_node->right == NULL) 
    {
        printf("Я думаю, что загаданный тобой актёр - %s\n", current_node->value);
        printf("Я угадал?\n");
        scanf("%5s", user_answer);
        getchar();

        if (strcmp(user_answer, "YES") == 0) 
        {
            printf("Ураа!\n");
        } 
        else 
        {
            printf("Ой-ей-ей! Ошибся, с кем не бывает :(\n");
            AkinatorCreateObject(tree, current_node);
        }
    } 
    else 
    {
        printf("Задаганный тобой актёр %s (YES/NO): ", current_node->value);
        scanf("%5s", user_answer);
        getchar();

        if (strcmp(user_answer, "YES") == 0) 
        {
            AkinatorFindNode(tree, current_node->right);
        } 
        else 
        {
            AkinatorFindNode(tree, current_node->left);
        }
    }
    
    return TREE_OK;
}

// добавляет новый объект, когда акинатор ошибается
TreeErr_t AkinatorCreateObject(Tree* tree, Node* current_node) 
{
    assert(current_node);
    assert(tree);

    char* new_object = (char*)calloc(LONG_BUFFER_CONST, sizeof(char));
    char* difference = (char*)calloc(LONG_BUFFER_CONST, sizeof(char));

    if (new_object == NULL || difference == NULL) 
    {
        printf("Calloc не сработал :(\n");
        return TREE_ERR_MEMORY;
    }

    printf("Кого бы ты хотел добавить?\n");
    scanf("%[^\n]", new_object);
    getchar();

    printf("В чём разница между %s и %s?\n", new_object, current_node->value);
    scanf("%[^\n]", difference);
    getchar();

    char* old_object = current_node->value; 

    current_node->value = difference;
    current_node->left  = NodeInit(new_object, NULL, NULL);
    current_node->right = NodeInit(old_object, NULL, NULL);
    tree->size += 2;

    printf("Новый актёр добавлен!\n\n");
    TreeDump(tree, "tree.dot");
    
    return TREE_OK;
}

// сохраняет дерево в текстовый файл
TreeErr_t TreeSaveToFile(Tree* tree, const char* filename) 
{
    assert(tree);
    assert(filename);

    FILE* file = fopen(filename, "w");
    if (file == NULL) 
    {
        return TREE_ERR_FILE;
    }

    TreeErr_t result = TreeSaveNodeToFile(tree->root, file);
    fclose(file);
    
    return result;
}

TreeErr_t TreeSaveNodeToFile(Node* node, FILE* filestream) 
{
    assert(filestream);
    
    if (node == NULL) 
    {
        return TREE_OK;
    }

    fprintf(filestream, "(\"");
    fprintf(filestream, "%s\"", node->value);
    
    if (node->left != NULL) 
    {
        fprintf(filestream, " ");
        TreeSaveNodeToFile(node->left, filestream);
    }
    
    if (node->right != NULL) 
    {
        fprintf(filestream, " ");
        TreeSaveNodeToFile(node->right, filestream);
    }
    
    fprintf(filestream, ")");
    
    return TREE_OK;
}

 // загружает дерево из файла
TreeErr_t TreeLoadFromFile(Tree* tree, const char* filename) 
{
    assert(tree);
    assert(filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("Не смогли открыть файл: %s\n", filename);
        return TREE_ERR_FILE;
    }
    
    char *buffer = (char*)calloc(LONG_BUFFER_CONST, sizeof(char));  
    if (buffer == NULL) 
    {
        printf("Calloc не сработал :(\n");
        return TREE_ERR_MEMORY;
    }
    
    char *ptr = buffer; 
    int c = 0;
    while ((c = fgetc(file)) != EOF) 
    {
        *ptr = (char)c; 
        ptr++;          
    }
    *ptr = '\0'; 
    
    fclose(file);

    CreateTreeFromStr(tree, buffer);
    
    free(buffer);
    
    return TREE_OK;
}

// создаёт дерево из строкового представления
TreeErr_t CreateTreeFromStr(Tree* tree, const char* str) 
{
    assert(tree);
    assert(str);

    const char* str_ptr = str;
    tree->root = CreateNodeFromStr(&str_ptr);
    
    if (tree->root == NULL) 
    {
        printf("Ошибка чтения файла\n");
        return TREE_ERR_FILE;
    }
    
    tree->size = CountNodes(tree->root);
    
    return TREE_OK;
}

// парсит строку и создаёт узел дерева
Node* CreateNodeFromStr(const char** str_ptr) 
{
    assert(str_ptr);
    
    SkipSpace(str_ptr);
    if (**str_ptr != '(') 
    {
        return NULL; 
    }
    
    (*str_ptr)++;
    SkipSpace(str_ptr);
    (*str_ptr)++;

    const char* name_start = *str_ptr;
    size_t name_len = 0;
    
    while (**str_ptr != '"' && **str_ptr != '\0') 
    {
        (*str_ptr)++;
        name_len++;
    }

    char* name = (char*)calloc(name_len + 1, sizeof(char));
    strncpy(name, name_start, name_len);
    name[name_len] = '\0';
    (*str_ptr)++;

    Node* node = NodeInit(name, NULL, NULL);
    
    SkipSpace(str_ptr);

    if (**str_ptr == '(') 
    {
        node->left = CreateNodeFromStr(str_ptr);
    }
    
    SkipSpace(str_ptr);
    
    if (**str_ptr == '(') 
    {
        node->right = CreateNodeFromStr(str_ptr);
    }
    
    SkipSpace(str_ptr);
    (*str_ptr)++;
    
    return node;
}

void SkipSpace(const char** str_ptr) 
{
    while (**str_ptr == ' ' || **str_ptr == '\n') 
    {
        (*str_ptr)++;
    }
    
    return ;
}

size_t CountNodes(Node* node) 
{
    if (node == NULL) 
    {
        return 0;
    }
    
    return 1 + CountNodes(node->left) + CountNodes(node->right);
}
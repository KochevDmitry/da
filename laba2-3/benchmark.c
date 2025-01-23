#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "patricia.h"

// Структура узла дерева
typedef struct Node {
    char *data;
    struct Node *left;
    struct Node *right;
} Node;

// Функция для создания нового узла
Node *createNode(char *data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = strdup(data); // Копируем строку
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Функция для вставки значения в дерево
Node *insert(Node *root, char *data) {
    if (root == NULL) {
        return createNode(data);
    }
    int compareResult = strcmp(data, root->data);
    if (compareResult < 0) {
        root->left = insert(root->left, data);
    } else if (compareResult > 0) {
        root->right = insert(root->right, data);
    }
    return root;
}

// Функция для поиска значения в дереве
Node *searchNode(Node *root, char *data) {
    if (root == NULL || strcmp(root->data, data) == 0) {
        return root;
    }
    if (strcmp(data, root->data) < 0) {
        return searchNode(root->left, data);
    }
    return searchNode(root->right, data);
}

// Вспомогательная функция для поиска минимального значения в дереве
Node *minValueNode(Node *node) {
    Node *current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Функция для удаления значения из дерева
Node *deleteNode(Node *root, char *data) {
    if (root == NULL) {
        return root;
    }
    int compareResult = strcmp(data, root->data);
    if (compareResult < 0) {
        root->left = deleteNode(root->left, data);
    } else if (compareResult > 0) {
        root->right = deleteNode(root->right, data);
    } else {
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root->data);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node *temp = root->left;
            free(root->data);
            free(root);
            return temp;
        }
        Node *temp = minValueNode(root->right);
        free(root->data);
        root->data = strdup(temp->data);
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

// Функция для освобождения памяти, выделенной для дерева
void freeTree(Node *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->data);
        free(root);
    }
}

// Выше была реализация бинарного дерева

int main(){
    unsigned long long val;
    FILE *file;
    Patricia* root = NULL;
    char input[5000];
    char word[4100];
    char val_str[65];
    int now_insert = 0;
    int now_search = 0;
    int now_remove = 0;
    file = fopen("tests.txt", "r");
    struct timeval start_time_2, end_time_2, start_time, end_time, start_time_3, end_time_3;
    double search_time_search_bin;
    double search_time_insert_bin;
    double search_time_remove_bin;
    double search_time_search_patricia;
    double search_time_insert_patricia;
    double search_time_remove_patricia;

    while (fgets(input, sizeof(input), file)) {
        if (input[0] == '+' && now_insert == 0){
            now_insert = 1;

            gettimeofday(&start_time, NULL); // начальное время

            sscanf(input, "+ %s %s\n", word, val_str);
            val = strtoull(val_str, NULL, 10);
            root = createPatricia(val, word);
        }
        else if (input[0] == '+'){
            sscanf(input, "+ %s %s\n", word, val_str);
            val = strtoull(val_str, NULL, 10);
            insertToPatricia(root, word, val, 0);
        }
        else if (input[0] == '-'){
            if (now_remove == 0){
                gettimeofday(&end_time_2, NULL); // конечное время
                long long start_ms_2 = start_time_2.tv_sec * 1000LL + start_time_2.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
                long long end_ms_2 = end_time_2.tv_sec * 1000LL + end_time_2.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
                search_time_search_patricia = (end_ms_2 - start_ms_2) / 1000.0; // искомо

                now_remove = 1;

                gettimeofday(&start_time_3, NULL); // начальное время
            }
            sscanf(input, "- %s\n", word);
            removeNode(root, word);
        }
        else {
            if (now_search == 0){
                gettimeofday(&end_time, NULL); // конечное время
                long long start_ms = start_time.tv_sec * 1000LL + start_time.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
                long long end_ms = end_time.tv_sec * 1000LL + end_time.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
                search_time_insert_patricia = (end_ms - start_ms) / 1000.0; // искомо

                now_search = 1;

                gettimeofday(&start_time_2, NULL);

            }
            sscanf(input, "%s\n", word);
            Patricia* node = search(root, word, -2);
        }
    }
    gettimeofday(&end_time_3, NULL); // конечное время
    long long start_ms_3 = start_time_3.tv_sec * 1000LL + start_time_3.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
    long long end_ms_3 = end_time_3.tv_sec * 1000LL + end_time_3.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
    search_time_remove_patricia = (end_ms_3 - start_ms_3) / 1000.0; // искомо

    fclose(file);

    now_insert = 0;
    now_search = 0;
    now_remove = 0;
    Node* root_bin = NULL;


    file = fopen("tests.txt", "r");

    struct timeval start_time_5, end_time_5, start_time_4, end_time_4, start_time_6, end_time_6;

    while (fgets(input, sizeof(input), file)) {
        if (input[0] == '+' && now_insert == 0){
            now_insert = 1;

            gettimeofday(&start_time_4, NULL); // начальное время

            sscanf(input, "+ %s %s\n", word, val_str);
            val = strtoull(val_str, NULL, 10);
            root_bin = createNode(word);
        }
        else if (input[0] == '+'){
            sscanf(input, "+ %s %s\n", word, val_str);
            val = strtoull(val_str, NULL, 10);
            insert(root_bin, word);
        }
        else if (input[0] == '-'){
            if (now_remove == 0){
                gettimeofday(&end_time_5, NULL); // конечное время
                long long start_ms_5 = start_time_5.tv_sec * 1000LL + start_time_5.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
                long long end_ms_5 = end_time_5.tv_sec * 1000LL + end_time_5.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
                search_time_search_bin = (end_ms_5 - start_ms_5) / 1000.0; // искомо

                now_remove = 1;

                gettimeofday(&start_time_6, NULL); // начальное время
            }
            sscanf(input, "- %s\n", word);
            deleteNode(root_bin, word);
        }
        else {
            if (now_search == 0){
                gettimeofday(&end_time_4, NULL); // конечное время
                long long start_ms_4 = start_time_4.tv_sec * 1000LL + start_time_4.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
                long long end_ms_4 = end_time_4.tv_sec * 1000LL + end_time_4.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
                search_time_insert_bin = (end_ms_4 - start_ms_4) / 1000.0; // искомо

                now_search = 1;

                gettimeofday(&start_time_5, NULL);

            }
            sscanf(input, "%s\n", word);
            Node* node = searchNode(root_bin, word);
        }
    }
    gettimeofday(&end_time_6, NULL); // конечное время
    long long start_ms_6 = start_time_6.tv_sec * 1000LL + start_time_6.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
    long long end_ms_6 = end_time_6.tv_sec * 1000LL + end_time_6.tv_usec / 1000; // преобразование из микросекунд в миллисекунды
    search_time_remove_bin = (end_ms_6 - start_ms_6) / 1000.0; // искомо

    printf("insert to patricia: %.4f\n", search_time_insert_patricia);
    printf("insert to bin tree: %.4f\n", search_time_insert_bin);

    printf("search in patricia: %.4f\n", search_time_search_patricia);
    printf("search in bin tree: %.4f\n", search_time_search_bin);

    printf("delete in patricia: %.4f\n", search_time_remove_patricia);
    printf("delete in bin tree: %.4f\n", search_time_remove_bin);
}
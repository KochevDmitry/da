#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patricia.h"

char toLower(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch + ('a' - 'A');
    }
    return ch;
}

void toLowercase(char str[500]) {
    for (int i = 0; i < 500; i++) {
        str[i] = toLower(str[i]);
    }
}


char* toBinary(unsigned long long value) {
    static char binary[65];
    for (int i = 63; i >= 0; --i) {
        binary[63 - i] = ((value >> i) & 1) + '0';
    }
    binary[64] = '\0';
    return binary;
}

char* stringToBinary(const char* str) {
    static char binary[1300];
    memset(binary, 0, sizeof(binary));
    // printf("str, binary %s %s\n", str, binary);
    int index = 0;
    for (int i = 0; i < strlen(str); ++i) {
        if (str[i] == '\n')
            continue;
        char lowerCh = toLower(str[i]);
        int asciiCode = (int)lowerCh;
        for (int j = 4; j >= 0; --j) { // 5
            binary[index++] = ((asciiCode >> j) & 1) + '0';
        }
    }
    return binary;
}

char* decodeBinaryString(char* binaryString) {
    size_t length = strlen(binaryString);

    
    char* decodedString = (char*)malloc((length / 5) + 1); 

    for (size_t i = 0, j = 0; i < length; i += 5, j++) {
        int charCode = 0;
        for (int k = 0; k < 5; k++) {
            charCode = (charCode << 1) | (binaryString[i + k] - '0');
        }
        decodedString[j] = 'a' + charCode - 1;
    }


    decodedString[length / 5] = '\0'; //???


    return decodedString;
}


void writePatriciaToFile(FILE* file, Patricia* node, int last_bit) {
    if (node == NULL || last_bit >= node->bit) {
        return;
    } 
    else {
        size_t str_size = strlen(node->word);

        fwrite(&str_size, sizeof(size_t), 1, file); // Записываем длину строки
        fwrite(node->word, sizeof(char), str_size, file); // Записываем строку


        fwrite(&node->data, sizeof(unsigned long long), 1, file); // Записываем число

        writePatriciaToFile(file, node->left, node->bit);
        writePatriciaToFile(file, node->right, node->bit);
    }
}

Patricia* readPatriciaFromFile(FILE* file, int* check_correct) {
    char str[1300];
    char binary_data[64]; 
    unsigned long long data;
    size_t string_length; 
    int have_root = 0;
    int now_word = 1;
    Patricia* root_new = NULL;
    int index = 0;
    char ch;

    while (fread(&string_length, sizeof(size_t), 1, file) == 1) {
        if (string_length > 256) {
            *check_correct = 0;
            return NULL;
        }
        fread(str, sizeof(char), string_length, file);
        str[string_length] = '\0';

        fread(&data, sizeof(unsigned long long), 1, file);

        if (have_root == 0) {
            root_new = createPatricia(data, str);
            have_root = 1;
        } else {
            insertToPatricia(root_new, str, data, 0);
        }
    }
    
    *check_correct = 1;
    return root_new;
}

int main() {
    unsigned long long val;
    Patricia* root = NULL;
    int flag_have_tree = 0;
    char input[1400];
    char word[500];
    char val_str[65];
    char path[256];

    while (fgets(input, sizeof(input), stdin)) {
        if (input[0] == '+') {
            sscanf(input, "+ %s %s", word, val_str);
            val = strtoull(val_str, NULL, 10);
            toLowercase(word);
            char* binary_word = word;
            // printf("%llu %s\n", val, binary_word);

            if (flag_have_tree == 1) {
                insertToPatricia(root, binary_word, val, 1);
            }
            else {
                root = createPatricia(val, binary_word);
                flag_have_tree = 1;
                printf("OK\n");
            }
        }
        else if (input[0] == '-') {
            if (root != NULL) {
                sscanf(input, "- %s\n", word);
                toLowercase(word);
                char* binary_word = word;
                if (root->bit == -1 && root->left == root && strcmp(binary_word, root->word) == 0) {
                    free(root->word);
                    free(root);
                    root = NULL;
                    printf("OK\n");
                    flag_have_tree = 0;
                }
                else
                    removeNode(root, binary_word);
            }
            else {
                printf("NoSuchWord\n");
            }
        }
        else if (input[0] == '?') {
            printPatriciaInAddress(root, -2);  
        }
        else if (input[0] == '!') {
            if (input[2] == 'S') {

                    sscanf(input, "! Save %s", path);
                    FILE* file = fopen(path, "wb");
                    
                    writePatriciaToFile(file, root, -2);
                    fclose(file);
                    printf("OK\n"); 
                
            }
            if (input[2] == 'L') {
                int checker_in_load;
                sscanf(input, "! Load %s", path);
                Patricia * new_root = NULL;
                FILE* file = fopen(path, "rb");
                
                new_root = readPatriciaFromFile(file, &checker_in_load);
                fclose(file);
                if (new_root == NULL && checker_in_load == 0){
                    printf("ERROR: incorrect data format");
                }
                else{
                    deletePatricia(root, -2);
                    root = new_root;
                    printf("OK\n");
                    flag_have_tree = 1;
                    
                    if (new_root == NULL) {
                        flag_have_tree = 0;
                    }
                }

            }
        }
        else {
            sscanf(input, "%s\n", word);
            toLowercase(word);
            char* binary_word = word;
            Patricia* node = search(root, binary_word, -2);
            // printf("%s, %s", binary_word, node->word);
            if (node == NULL) {
                printf("NoSuchWord\n");
            }
            else if (strcmp(binary_word, node->word) == 0) {
                printf("OK: %llu\n", node->data);
            }
            else {
                printf("NoSuchWord\n");
            }
        }
    }
    deletePatricia(root, -2);
}


//     size_t elements_read = fread(str, sizeof(char), string_length, file);
//     if (elements_read != string_length) {
//         if (feof(file)) {
//             printf("Достигнут конец файла до чтения ожидаемого количества элементов.\n");
//         } else if (ferror(file)) {
//             perror("Произошла ошибка при чтении файла");
//         }
//     } else {
//         printf("Чтение прошло успешно.\n");
//     }

    // file = fopen("example.txt", "wb");
    // if (file == NULL) {
    //     perror("Не удалось открыть файл");
    //     return 1;
    // }
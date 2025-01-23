#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Patricia {
    unsigned long long data;
    int bit;
    char* word;

    struct Patricia* left;
    struct Patricia* right;
} Patricia;


Patricia* createPatricia(unsigned long long value, char* str) {
    Patricia* root = (Patricia*)malloc(sizeof(Patricia));
    root->data = value;
    root->word = (char*)malloc((strlen(str) + 1) * sizeof(char));
    strcpy(root->word, str);
    root->bit = -1;
    root->left = root;
    root->right = NULL;
    return root;
}

void printOneNode(Patricia* node) {
    printf("Address: %p\n", (void*)node);
    printf("Address of left: %p\n", (void*)node->left);
    printf("Address of right: %p\n", (void*)node->right);
    printf("bit: %d\n", node->bit);
    printf("word: %s\n", node->word);
    printf("data: %llu\n", node->data);
}

Patricia* search(Patricia* node, char* str, int lastbit) {
    // printOneNode(node);
    if (node == NULL) {
        return NULL;
    }
    if (node->bit <= lastbit) {
        return node;
    } 
    else if (node->bit == -1) {
        return search(node->left, str, -1);
    } 
    else {
        if (strlen(str) < node->bit / 5){
            return search(node->left, str, node->bit);
        }
        else{
            int current_bit = (((int)str[node->bit / 5] >> (4 - node->bit % 5)) & 1);
            // printf("current_bit: %d\n", current_bit);
            if (current_bit == 0 || strlen(str) <= node->bit / 5) {
                return search(node->left, str, node->bit);
            } else if (current_bit == 1) {
                return search(node->right, str, node->bit);
            }
        }
    }
    return NULL;
}

Patricia* searchNodeForInsert(Patricia* node, Patricia* searched, int searched_bit,
                                    Patricia* parent, int lastbit) {
    if (node->bit <= lastbit) {
        return parent;
    } else if (node->bit == -1) {
        if (node->bit < searched_bit && node->left->bit > searched_bit) {
            return node;
        }
        return searchNodeForInsert(node->left, searched, searched_bit, node, -1);
    } else {
        if (strlen(searched->word) < node->bit / 5){
            if (node->bit < searched_bit && node->left->bit > searched_bit) {
                return node;
            }
            return searchNodeForInsert(node->left, searched, searched_bit, node, node->bit);
        }
        else {
            int current_bit = (((int)searched->word[node->bit / 5] >> (4 - node->bit % 5)) & 1);
            if (current_bit == 0) {
                if (node->bit < searched_bit && node->left->bit > searched_bit) {
                    return node;
                }
                return searchNodeForInsert(node->left, searched, searched_bit, node, node->bit);
            } else if (current_bit == 1) {
                if (node->bit < searched_bit && node->right->bit > searched_bit) {
                    return node;
                }
                return searchNodeForInsert(node->right, searched, searched_bit, node, node->bit);
            }
        }
    }
    return NULL;
}

void insertToPatricia(Patricia* node, char* str, unsigned long long value, int ok_or_not) {
    Patricia* close = search(node, str, -2);    
    // printf("%s, %s\n", close->word, str);    

    if (strcmp(close->word, str) == 0) {
        if (ok_or_not)
            printf("Exist\n");
    } else {
        int searched_bit = 0;
        int index = 0;
        while (str[index] == close->word[index]) {
            index += 1;
        }
        if (index == strlen(close->word) && index < strlen(str)){ // Для кол-во бит = 5
            int ascii_code = (int)str[index];
            int position = 0;
            for (int i = 4; i >= 0; --i) { 
                if ((ascii_code >> i) & 1) {    
                    position = 4 - i;
                    break; 
                }
            }
            searched_bit = index * 5 + position;
        }
        else if (index < strlen(close->word) && index == strlen(str)){ // Для кол-во бит = 5
            int ascii_code = (int)close->word[index];
            int position = 0;
            for (int i = 4; i >= 0; --i) { 
                if ((ascii_code >> i) & 1) {    
                    position = 4 - i;
                    break; 
                }
            }
            searched_bit = index * 5 + position;
        }
        else{
            int ascii_code_a = (int)str[index];
            int ascii_code_b = (int)close->word[index];
            int position = 0;
            for (int i = 4; i >= 0; --i)  { 
                if (((ascii_code_a >> i) & 1) != ((ascii_code_b >> i) & 1)) { 
                    position = 4 - i;
                    break; 
                }
            }
            searched_bit = index * 5 + position;
        }
        // printf("searched_bit: %d\n", searched_bit);

        Patricia* parent_of_new = searchNodeForInsert(node, close, searched_bit, NULL, -2);
        Patricia* new_node = (Patricia*)malloc(sizeof(Patricia));
        new_node->data = value;
        new_node->word = (char*)malloc((strlen(str) + 1) * sizeof(char));
        strcpy(new_node->word, str);
        new_node->bit = searched_bit;

        if (parent_of_new->bit == -1) {
            if (strlen(str) <= searched_bit / 5){
                new_node->left = new_node;
                new_node->right = parent_of_new->left;
                parent_of_new->left = new_node;
            }
            else{
                // printf("here\n");
                int current_bit = (((int)str[searched_bit / 5] >> (4 - searched_bit % 5)) & 1);
                // printf("current_bit: %d\n", current_bit);
                // printf("%d\n", ((int)'b' >> 1) & 1);
                if (current_bit == 0) {
                    new_node->left = new_node;
                    new_node->right = parent_of_new->left;
                    parent_of_new->left = new_node;
                } else if (current_bit == 1) {
                    new_node->right = new_node;
                    new_node->left = parent_of_new->left;
                    parent_of_new->left = new_node;
                }
            }
        } else {
            int current_bit = ((str[searched_bit / 5] >> (4 - searched_bit % 5)) & 1);
            if (current_bit == 0 || strlen(str) <= searched_bit / 5) {
                new_node->left = new_node;
                int current_bit_2 = (((int)str[parent_of_new->bit / 5] >> (4 - parent_of_new->bit % 5)) & 1);
                if (current_bit_2 == 0 || strlen(str) <= parent_of_new->bit / 5) {
                    new_node->right = parent_of_new->left;
                    parent_of_new->left = new_node;
                } else if (current_bit_2 == 1) {
                    new_node->right = parent_of_new->right;
                    parent_of_new->right = new_node;
                } 
            } else if (current_bit == 1) {
                new_node->right = new_node;
                int current_bit_2 = (((int)str[parent_of_new->bit / 5] >> (4 - parent_of_new->bit % 5)) & 1);
                if (current_bit_2 == 0 || strlen(str) <= parent_of_new->bit / 5) {
                    new_node->left = parent_of_new->left;
                    parent_of_new->left = new_node;
                } else if (current_bit_2 == 1) {
                    new_node->left = parent_of_new->right;
                    parent_of_new->right = new_node;
                } 
            }
        }
        if (ok_or_not)
            printf("OK\n");
    }
}

Patricia* searchBackLinkToNode(Patricia* node, Patricia* searched, Patricia* parent, int lastbit) {
    if (node->bit <= lastbit) {
        return parent;
    } else if (node->bit == -1) {
        return searchBackLinkToNode(node->left, searched, node, -1);
    } else {
        int current_bit = (((int)searched->word[node->bit / 5] >> (4 - node->bit % 5)) & 1);
        // printf("node->bit: %d, searched->word: %s, searched->word[node->bit]: %c, strlen(searched->word): %ld\n", node->bit, searched->word, searched->word[node->bit], strlen(searched->word));
        if (current_bit == 0 || strlen(searched->word) <= node->bit / 5) {
            // printf("here0\n");
            return searchBackLinkToNode(node->left, searched, node, node->bit);
        } else if (current_bit == 1) {
            // printf("here1\n");
            return searchBackLinkToNode(node->right, searched, node, node->bit);
        } 
    }
    return NULL;
}

Patricia* searchDirectLinkToNode(Patricia* node, Patricia* searched, Patricia* parent, int lastbit) {
    if (node == searched) {
        return parent;
    } else if (node->bit == -1) {
        return searchDirectLinkToNode(node->left, searched, node, -1);
    } else {
        int current_bit = (((int)searched->word[node->bit / 5] >> (4 - node->bit % 5)) & 1);
        if (current_bit == 0 || strlen(searched->word) <= node->bit / 5) {
            return searchDirectLinkToNode(node->left, searched, node, node->bit);
        } else if (current_bit == 1) {
            return searchDirectLinkToNode(node->right, searched, node, node->bit);
        }
    }
    return NULL;
}

void removeNode(Patricia* root, char* str) {
    Patricia* X = search(root, str, -2);
        
    if (X == NULL || strcmp(X->word, str) != 0) {
        printf("NoSuchWord\n");
        return;
    }

    if (X->bit == -1 && X->left == root) {
        free(root);
        root = NULL;
        printf("OK\n");
    } else if (X->right == X || X->left == X) {
        Patricia* Parent = searchDirectLinkToNode(root, X, NULL, -2);
        if (X->right == X) {
            if (Parent->left == X)
                Parent->left = X->left;
            else
                Parent->right = X->left;
        } else {
            if (Parent->left == X)
                Parent->left = X->right;
            else
                Parent->right = X->right;
        }
        free(X->word);
        free(X);
        printf("OK\n");
    } else {
        Patricia* Q = searchBackLinkToNode(root, X, NULL, -2);
        // printf("---------\n");
        Patricia* P = searchBackLinkToNode(root, Q, NULL, -2);
        Patricia* M = searchDirectLinkToNode(root, Q, NULL, -2);
        // printf("Q: %s; P: %s; M: %s; X: %s;\n", Q->word, P->word, M->word, X->word);
        Patricia* N;
        size_t new_size = strlen(Q->word) + 1;
        char* temp = realloc(X->word, new_size);
        X->word = temp;
        strcpy(X->word, Q->word);
        X->data = Q->data;

        if (P->left == Q)
            P->left = X;
        else
            P->right = X;

        if (Q->left == X)
            N = Q->right;
        else
            N = Q->left;

        if (M->left == Q)
            M->left = N;
        else
            M->right = N;

        free(Q->word);
        free(Q);
        printf("OK\n");
    }
}

void printPatricia(Patricia* root, int level, int last_bit) {
    if (root == NULL) {
        return;
    }

    if (last_bit >= root->bit)
        return;

    if (root->bit > last_bit)
        printPatricia(root->right, level + 1, root->bit);
    
    for (int i = 0; i < level; ++i) {
        printf("  "); 
    }
    printf("%s\n", root->word);

    if (root->bit > last_bit)
        printPatricia(root->left, level + 1, root->bit);
}

void printPatriciaInAddress(Patricia *root, int last_bit) {
    if (root == NULL) {
        return;
    }

    if (last_bit >= root->bit) {
        return;
    }

    if (root->bit > last_bit) {
        printPatriciaInAddress(root->left, root->bit);
    }
    
    printOneNode(root);

    if (root->bit > last_bit) {
        printPatriciaInAddress(root->right, root->bit);
    }
}

void deletePatricia(Patricia* node, int last_bit) {
    if (node == NULL) {
        return;
    }
    if (last_bit >= node->bit)
        return;
    
    deletePatricia(node->left, node->bit);
    deletePatricia(node->right, node->bit);

    free(node->word);
    free(node);
}


char toLower(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch + ('a' - 'A');
    }
    return ch;
}

void toLowercase(char str[500]) {
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
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
    // binary[index] = '\0';
    // while (index < 1600) {//2100
    //     binary[index++] = '0';
    // }
    // printf("str, binary %s %s\n", str, binary);
    return binary;
}

char* decodeBinaryString(char* binaryString) {
    size_t length = strlen(binaryString);
    // printf("%s, ", binaryString);
    
    char* decodedString = (char*)malloc((length / 5) + 1); // Каждые 5 бит превращаются в символ, плюс 1 для нуль-терминатора

    for (size_t i = 0, j = 0; i < length; i += 5, j++) {
        int charCode = 0;
        for (int k = 0; k < 5; k++) {
            charCode = (charCode << 1) | (binaryString[i + k] - '0');
        }
        decodedString[j] = 'a' + charCode - 1;
    }


    decodedString[length / 5] = '\0'; //???
    // printf(" %s\n", decodedString);

    return decodedString;
}


void writePatriciaToFile(FILE* file, Patricia* node, int last_bit) {
    if (node == NULL || last_bit >= node->bit) {
        return;
    } 
    else {
        size_t str_size = strlen(node->word);
        // printf("%s, %s\n", node->word, stringToBinary(node->word));
        // printf("aaa\n");
        fwrite(&str_size, sizeof(size_t), 1, file); // Записываем длину строки
        fwrite(node->word, sizeof(char), str_size, file); // Записываем строку
        // fprintf(file, "%s\n", node->word);

        // fprintf(file, "\n");

        fwrite(&node->data, sizeof(unsigned long long), 1, file); // Записываем число

        // fwrite(&node->data, sizeof(node->data), 1, file);
        // fprintf(file, "\n");


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

    // while ((ch = fgetc(file)) != EOF) {
    //     if (ch == '\n') {
    //         binary_word[index] = '\0';
    //         char* decodedString = decodeBinaryString(binary_word);
    //         index = 0;
    //         fgets(binary_data, sizeof(binary_data), file);
    //         data = strtoull(binary_data, NULL, 2);
    //         // printf("%s %sqqq\n", binary_word, binary_data);
    //         if (have_root == 0) {
    //             root_new = createPatricia(data, decodedString);
    //             have_root = 1;
    //         } else {
    //             insertToPatricia(root_new, decodedString, data, 0);
    //         }
    //         ch = fgetc(file); // чтобы считать ентер
    //         free(decodedString);
    //     } else {
    //         if (index >= 1281){
    //             *check_correct = 0;
    //             return NULL;
    //         }
    //         binary_word[index++] = ch;
    //     }
    // }
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
// Patricia* readPatriciaFromFile(FILE* file) {
//     char binary_word[1301]; 
//     char binary_data[66]; 
//     unsigned long long data = 1000;
//     int have_root = 0;
//     Patricia* root_new = NULL;

//     while (fgets(binary_word, sizeof(binary_word), file)) {
//         size_t length = strlen(binary_word);
//         if (binary_word[length - 1] == '\n') {
//             binary_word[length - 1] = '\0';
//         }
//         else
//             return NULL;

//         fgets(binary_data, sizeof(binary_data), file);
//         length = strlen(binary_data);
//         if (binary_data[length - 1] == '\n') {
//             binary_data[length - 1] = '\0';
//         }
//         else
//             return NULL;

//         data = strtoull(binary_data, NULL, 2);

//         // printf("%s %llu qqqq\n", binary_word, data);
//         if (have_root == 0) {
//             root_new = createPatricia(data, binary_word);
//             have_root = 1;
//         }
//         else
//             insertToPatricia(root_new, binary_word, data, 0);

//     }

//     return root_new;
// }

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
                // if (root == NULL) {
                //     printf("ERROR: root is empty\n");
                // }
                // else {
                    sscanf(input, "! Save %s", path);
                    FILE* file = fopen(path, "wb");
                    
                    writePatriciaToFile(file, root, -2);
                    fclose(file);
                    printf("OK\n"); 
                
                    // printf("ERROR: unable to open file\n");
                    
                // }
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
                // else{
                //     printf("ERROR: file not correct or empty\n");
                // }
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
        // memset(input, 0, sizeof(input));
        // memset(word, 0, sizeof(word));
        // memset(val_str, 0, sizeof(val_str));
        // memset(path, 0, sizeof(path));
    }
    deletePatricia(root, -2);
}

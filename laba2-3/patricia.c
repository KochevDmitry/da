#include "patricia.h"

Patricia* createPatricia(unsigned  long long value, char* str) {
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
        // printf("NoSuchWord\n");
        return;
    }

    if (X->bit == -1 && X->left == root) {
        free(root);
        root = NULL;
        // printf("OK\n");
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
        // printf("OK\n");
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
        // printf("OK\n");
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
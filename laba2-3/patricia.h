#ifndef PATRICIA_H
#define PATRICIA_H

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

Patricia* createPatricia(unsigned long long value, char* str);
void printOneNode(Patricia* node);
Patricia* search(Patricia* node, char* str, int lastbit);
Patricia* searchNodeForInsert(Patricia* node, Patricia* searched, int searched_bit, Patricia* parent, int lastbit);
void insertToPatricia(Patricia* node, char* str, unsigned long long value, int ok_or_not);
Patricia* searchBackLinkToNode(Patricia* node, Patricia* searched, Patricia* parent, int lastbit);
Patricia* searchDirectLinkToNode(Patricia* node, Patricia* searched, Patricia* parent, int lastbit);
void removeNode(Patricia* root, char* str);
void printPatricia(Patricia* root, int level, int last_bit);
void printPatriciaInAddress(Patricia *root, int last_bit);
void deletePatricia(Patricia* node, int last_bit);

#endif /* PATRICIA_H */

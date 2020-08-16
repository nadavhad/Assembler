#include <stdlib.h>
#include <string.h>
#include "externusage.h"

struct Node {
    struct Node *next;
    ExternUsage data;
};

static struct Node *_head = NULL;

struct Node *initNode(char label[MAX_LABEL_LENGTH], int usage) {
    struct Node *node = (struct Node *) malloc(sizeof(struct Node));
    node->next = NULL;
    strcpy(node->data.externLabel, label);
    node->data.usageAddress = usage;
    return node;
}

int addUsage(char externLabel[MAX_LABEL_LENGTH], int usageAddress) {
    if (_head == NULL) {
        _head = initNode(externLabel, usageAddress);
    } else {
        struct Node *iter = _head;
        while (iter->next == NULL) {
            iter = iter->next;
        }
        iter->next = initNode(externLabel, usageAddress);
    }
    return 0;
}

void clearExternUsagesTable() {
    struct Node *iterator = _head;
    while (iterator != NULL) {
        struct Node *next = iterator->next;
        free(iterator);
        iterator = next;
    };
    _head = NULL;
}

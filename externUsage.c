#include <stdlib.h>
#include <string.h>
#include "externUsage.h"

/**
 * A linked-list Node for extern usage data
 */
struct Node {
    struct Node *next;
    ExternUsage data;
};

/**
 * Create a new Node with the logged usage data
 */
struct Node *initNode(char label[MAX_LABEL_LENGTH], int usage);

/**
 * The head of the linked list
 */
static struct Node *_head = NULL;


struct Node *initNode(char label[MAX_LABEL_LENGTH], int usage) {
    /* allocate Node data */
    struct Node *node = (struct Node *) malloc(sizeof(struct Node));
    /* assign usage data */
    node->next = NULL;
    strcpy(node->data.externLabel, label);
    node->data.usageAddress = usage;
    return node;
}

int addUsage(char externLabel[MAX_LABEL_LENGTH], int usageAddress) {
    /* init the list if still empty */
    if (_head == NULL) {
        _head = initNode(externLabel, usageAddress);
    } else {
        struct Node *iter = _head;
        /* iterate to the end of the list */
        while (iter->next != NULL) {
            iter = iter->next;
        }
        /* add the new Node to the list*/
        iter->next = initNode(externLabel, usageAddress);
    }
    return 0;
}

void clearExternUsagesTable() {
    struct Node *iterator = _head;
    /* free the list */
    while (iterator != NULL) {
        struct Node *next = iterator->next;
        free(iterator);
        iterator = next;
    }
    _head = NULL;
}

int startExternUsageIteration(void **iterator, ExternUsage *data) {
    /* expose _head */
    *iterator = _head;
    /* copy the usage data itelf */
    if (*iterator != NULL) {
        memcpy(data, &((struct Node *) *iterator)->data, sizeof(ExternUsage));
    }
    return 0;
}

int getExternUsageNext(void **iterator, ExternUsage *data) {
    /* expose the next Node */
    *iterator = ((struct Node *) *iterator)->next;
    /* copy the usage data itelf */
    if (*iterator != NULL) {
        memcpy(data, &((struct Node *) *iterator)->data, sizeof(ExternUsage));
    }
    return 0;
}

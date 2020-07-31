#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbolTable.h"
#include "errorlog.h"
#include "structs.h"

/*TODO: free all realocatable space, (also on Yotam's malloc) */

struct Node {
    SymbolData data;
    struct Node *next;
};

static struct Node *_head = NULL;

struct Node *lookUpNode(char name[MAX_LABEL_LENGTH], struct Node **lastNode);

struct Node *createNode(char name[MAX_LABEL_LENGTH], int value, LineType type, enum bool isEntry) {
    struct Node *node = (struct Node *) malloc(sizeof(struct Node));
    node->next = NULL;
    strcpy(node->data.name, name);
    node->data.value = value;
    node->data.type = type;
    node->data.isEntry = isEntry;

    return node;
}

int addSymbol(char name[MAX_LABEL_LENGTH], int value, LineType type, enum bool isEntry) {
    struct Node *lastNode;
    if (_head == NULL) {
        _head = createNode(name, value, type, isEntry);
        return 0;
    }

    /* Check if already exists (and log error), Otherwise - add at the end.*/
    if (lookUpNode(name, &lastNode) != NULL) {
        char buf[100];
        sprintf(buf, "Label '%s' already exists", name);
        logError(getLineNumber(), buf);
        return -1;
    }

    lastNode->next = createNode(name, value, type, isEntry);
    return 0;
}

struct Node *lookUpNode(char name[MAX_LABEL_LENGTH], struct Node **lastNode) {
    struct Node *iterator = NULL;
    iterator = _head;
    do {
        if (strcmp(iterator->data.name, name) == 0) {
            return iterator;
        }
        if (iterator->next == NULL) {
            break; /* Last node */
        }
        iterator = iterator->next;
    } while (1);
    *lastNode = iterator;
    return NULL;
}

int lookUp(char name[MAX_LABEL_LENGTH], SymbolData *symbolData) {
    struct Node *lastNode;
    struct Node *node = lookUpNode(name, &lastNode);
    if (node == NULL) {
        return -1;
    }
    memcpy(symbolData, &node->data, sizeof(SymbolData));
    return 0;
}

int updateSymbol(char name[MAX_LABEL_LENGTH], int value) {
    struct Node *lastNode;
    struct Node *iterator = lookUpNode(name, &lastNode);
    if (iterator == NULL) {
        logError(getLineNumber(), "Label not found.");
        return -1;
    }
    iterator->data.value = value;
    return 0;
}
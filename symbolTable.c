#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbolTable.h"
#include "errorLog.h"
#include "structs.h"
#include "state.h"
#include "errorLog.h"

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

int addSymbol(char name[MAX_LABEL_LENGTH], int value, SymbolType type, enum bool isEntry) {
    struct Node *lastNode;
    struct Node *existingNode;
    if (strlen(name) == 0) { /* if label is empty, don't add it to the table */
        return 0;
    }
    if (_head == NULL) {
        _head = createNode(name, value, type, isEntry);
        return 0;
    }

    /* Check if already exists (and log error), Otherwise - add at the end.*/
    existingNode = lookUpNode(name, &lastNode);
    if (existingNode != NULL) {
        if ((type == ST_EXTERNAL) && (existingNode->data.type == ST_EXTERNAL)) {
            return 0;
        }
        ERROR_RET((_, "Label '%s' already exists", name));
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

int setEntrySymbol(char *label) {
    struct Node *lastNode;
    struct Node *iterator = lookUpNode(label, &lastNode);
    if (iterator == NULL) {
        ERROR_RET((_, "Label not found: %s", label));
    }
    iterator->data.isEntry = TRUE;
    return 0;
}

int incrementDataSymbolsOffset(int icf) {
    struct Node *iterator = _head;
    while (iterator != NULL) {
        if (iterator->data.type == ST_DATA) {
            iterator->data.value += icf;
        }
        iterator = iterator->next;
    }
    return 0;
}

void clearSymbolTable() {
    struct Node *iterator = _head;
    while (iterator != NULL) {
        struct Node *next = iterator->next;
        free(iterator);
        iterator = next;
    };
    _head = NULL;
}

enum bool isSymbolTableComplete() {
    return (getState()->ICF >= 0) || (getState()->DCF >= 0);
}

int startSymbolTableIteration(void **iterator, SymbolData *data) {
    *iterator = _head;
    if (*iterator != NULL) {
        memcpy(data, &((struct Node *) *iterator)->data, sizeof(SymbolData));
    }
    return 0;
}

int getSymbolTableNext(void **iterator, SymbolData *data) {
    *iterator = ((struct Node *) *iterator)->next;
    if (*iterator != NULL) {
        memcpy(data, &((struct Node *) *iterator)->data, sizeof(SymbolData));
    }
    return 0;
}


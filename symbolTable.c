#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbolTable.h"
#include "errorLog.h"

/**
 * A linked-list Node for symbol data
 */
struct Node {
    SymbolData data;
    struct Node *next;
};

/**
 * Create a new Node with symbol data
 */
struct Node *createNode(char name[MAX_LABEL_LENGTH], int value, LineType type, enum bool isEntry);

/**
 * Finds the Node with the symbol name.
 * If no matching Node is found, returns NULL and lastNode is the last Node in the list.
 */
struct Node *lookUpNode(char name[MAX_LABEL_LENGTH], struct Node **lastNode);

/**
 * The head of the linked list
 */
static struct Node *_head = NULL;

struct Node *createNode(char name[MAX_LABEL_LENGTH], int value, LineType type, enum bool isEntry) {
    /* allocate Node data */
    struct Node *node = (struct Node *) malloc(sizeof(struct Node));
    /* assign usage data */
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
    /* init the list if still empty */
    if (_head == NULL) {
        _head = createNode(name, value, type, isEntry);
        return 0;
    }

    /* Check if already exists (and log error), Otherwise - add at the end.*/
    existingNode = lookUpNode(name, &lastNode);
    if (existingNode != NULL) {
        if ((type == ST_EXTERNAL) && (existingNode->data.type == ST_EXTERNAL)) {
            /* importing (via ".extern") the same symbol twice isn't an error */
            return 0;
        }
        /* symbol already exists, symbols must be unique */
        ERROR_RET((_, "Label '%s' already exists", name));
    }
    /* add the new Node to the list*/
    lastNode->next = createNode(name, value, type, isEntry);
    return 0;
}

struct Node *lookUpNode(char name[MAX_LABEL_LENGTH], struct Node **lastNode) {
    struct Node *iterator = NULL;
    iterator = _head;
    if (iterator == NULL) {
        /* list is empty*/
        return NULL;
    }
    do {
        if (strcmp(iterator->data.name, name) == 0) {
            /* we found the symbol */
            return iterator;
        }
        if (iterator->next == NULL) {
            /* Last node */
            break;
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
        /* not found */
        return -1;
    }
    /* found, ptr-return the data */
    memcpy(symbolData, &node->data, sizeof(SymbolData));
    return 0;
}

int setEntrySymbol(char *label) {
    struct Node *lastNode;
    /* find symbol */
    struct Node *iterator = lookUpNode(label, &lastNode);
    if (iterator == NULL) {
        /* tried to mark a non-existant symbol as entry */
        ERROR_RET((_, "Label not found: %s", label));
    }
    /* mark symbol as entry */
    iterator->data.isEntry = TRUE;
    return 0;
}

int incrementDataSymbolsOffset(int icf) {
    struct Node *iterator = _head;
    while (iterator != NULL) {
        /* add ICF to address of all data symbols */
        if (iterator->data.type == ST_DATA) {
            iterator->data.value += icf;
        }
        iterator = iterator->next;
    }
    return 0;
}

void clearSymbolTable() {
    struct Node *iterator = _head;
    /* free the list */
    while (iterator != NULL) {
        struct Node *next = iterator->next;
        free(iterator);
        iterator = next;
    };
    _head = NULL;
}

enum bool isSymbolTableComplete() {
    /* if ICF or DCF are initialized, then we have all symbol data */
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

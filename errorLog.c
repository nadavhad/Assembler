#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define MAX_ERROR_LENGTH 200
/**
 * A linked-list Node for error saving
 */
struct Node {
    char string[MAX_ERROR_LENGTH];
    struct Node *next;
};

/**
 * Print the errors to stderr
 */
static void errorLog(const char *string);

/**
 * Create a new Node containing the given string
 */
static struct Node *createNode(const char *string);

/**
 * The head of the linked list
 */
static struct Node *_head = NULL;

/**
 * Create a new Node containing the given string
 */
static struct Node *createNode(const char *string) {
    struct Node *node;
    node = (struct Node *) malloc(sizeof(struct Node));
    node->next = NULL;
    strncpy(node->string, string, MAX_ERROR_LENGTH);
    return node;
}

/**
 * Log an error to the error list
 * @param lineNumber the line number in the assembly file
 * @param errorStr the string that describes the error
 */
void logError(int lineNumber, char *errorStr) {
    char errorWithLine[MAX_ERROR_LENGTH + 10];
    struct Node *added;
    struct Node *iterator = NULL;
    /* format error with line number appropriately */
    sprintf(errorWithLine, "%d: %s", lineNumber, errorStr);
    /* create a new node with the currently logged error */
    added = createNode(errorWithLine);
    /* init the list if still empty */
    if (_head == NULL) {
        _head = createNode("Errors:");
    }
    /* iterate to the end of the list */
    iterator = _head;
    while (iterator->next != NULL) {
        iterator = iterator->next;
    }
    /* add the new Node to the list*/
    iterator->next = added;
}

/**
 * Print the errors to stderr
 */
static void errorLog(const char *string) {
    fprintf(stderr, "%s\n", string);
}

/**
 * Check how many errors were logged
 * @return the number of errors
 */
int numErrors() {
    struct Node *node = _head;
    int counter = 0;
    /* count how many errors we have */
    while (node != NULL) {
        node = node->next;
        counter++;
    }
    return counter;
}
/**
 * Print all logged errors to stderr
 */
void flush() {
    struct Node *node = _head;
    while (node != NULL) {
        /* print each error to stderr */
        errorLog(node->string);
        node = node->next;
    }
}
/**
 * Clears the error log
 * */
void clearErrorLog() {
    struct Node *iterator = _head;
    /* free the list */
    while (iterator != NULL) {
        struct Node *next = iterator->next;
        free(iterator);
        iterator = next;
    };
    _head = NULL;
}


/**
 * Converts an AddressingType value to a string value for printing
 */
char *addressingTypeStr(enum AddressingType addressingType) {
    static char buf[10];
    /* return a string with the given addressing type */
    switch (addressingType) {
        case AT_IMMEDIATE:
            strcpy(buf, "IMMEDIATE");
            break;
        case AT_DIRECT:
            strcpy(buf, "DIRECT");
            break;
        case AT_RELATIVE:
            strcpy(buf, "RELATIVE");
            break;
        case AT_REGISTER:
            strcpy(buf, "REGISTER");
            break;
        case AT_UNSET:
            strcpy(buf, "UNSET");
            break;

    }
    return buf;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ERROR_LENGTH 200
struct Node {
    char string[MAX_ERROR_LENGTH];
    struct Node *next;
};

static struct Node *_head = NULL;

static struct Node *createNode(const char *string) {
  struct Node *node;
  node = (struct Node *) malloc(sizeof(struct Node));
  node->next = NULL;
  strncpy(node->string, string, MAX_ERROR_LENGTH);
  return node;
}

void logError(const char *error) {
  struct Node *added = createNode(error);
  struct Node *iterator = NULL;
  if(_head == NULL) {
      _head = createNode("Errors:");
  }
    iterator = _head;
  while (iterator->next != NULL) {
    iterator = iterator->next;
  }
  iterator->next = added;
}

static void errorLog(const char *string) {
  fprintf(stderr, "%s\n", string);
}

void flush() {
  struct Node *node = _head;
  while (node != NULL) {
    errorLog(node->string);
    node = node->next;
  }
}
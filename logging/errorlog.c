#include <stdio.h>
#include <stdlib.h>

struct Node {
    const char *string;
    struct Node *next;
};

static struct Node *first = NULL;

static struct Node *createNode(const char *string) {
  struct Node *node;
  node = (struct Node *) malloc(sizeof(struct Node));
  node->next = NULL;
  node->string = string;
}

void log(const char *error) {
  struct Node *added = createNode(error);
  struct Node *iterator = NULL;
  if(first == NULL) {
    first = createNode("Errors:");
  }
    iterator = first;
  while (iterator->next != NULL) {
    iterator = iterator->next;
  }
  iterator->next = added;
}

static void errorLog(const char *string) {
  fprintf(stderr, "%s\n", string);
}

void flush() {
  struct Node *node = first;
  while (node != NULL) {
    errorLog(node->string);
    node = node->next;
  }
}
#ifndef TISPY
#define TISPY
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKEN_SIZE 2048
#define DATA_STRIPE 40
#define INITIAL_HEAP_SIZE 32
typedef enum {
  STATEMENT_FUNCTION = 1,
  EXPRESSION_BOOLEAN,
  EXPRESSION_STRING,
  EXPRESSION_INTEGER,
  EXPRESSION_DOUBLE,
  EXPRESSION_TABLE,
  EXPRESSION_FUNCTION,
  EXPRESSION_UNDEFINED
} Type;
typedef struct Token {
  Type type;
  char *content;
} Token;
typedef struct TreeNode {
  struct TreeNode *children;
  struct TreeNode *branch;
  Token token;
} TreeNode;
typedef struct Data {
  Type type;
  unsigned char value[DATA_STRIPE];
  struct Data *next;
} Data;
typedef struct {
  int size;
  Data table[];
} DataTable;
typedef struct {
  TreeNode *root;
  DataTable *heap;
  Data *stack;
} TispyState;
typedef enum { FILE_INPUT = 1, STRING_INPUT } InputKind;
typedef struct {
  InputKind kind;
  struct {
    FILE *file;
    char *string;
  } data;
} Input;
static Data* popStack() {
}
static char igetc(Input *i) {
  return i->kind == FILE_INPUT ? fgetc(i->data.file) : i->data.string[0]++;
}
static void logError(char *content) {
  printf("Tispy interpreter:\n[ERROR]: %s\n", content);
}
static int isInt(char *content) {
  for (int i = 0; i < strlen(content); i++) {
    if (!isdigit(content[i]))
      return 0;
  }
  return 1;
}
static Type getType(char *content) {
  if (!strcmp(content, "true") || !strcmp(content, "false")) {
    return EXPRESSION_BOOLEAN;
  }
  if (content[0] == '"' && content[strlen(content)] == '"') {
    return EXPRESSION_STRING;
  }
  if (isInt(content)) {
    return EXPRESSION_INTEGER;
  }
  return EXPRESSION_UNDEFINED;
}
TreeNode *parse(Input *i) {
  char character = ' ';
  char currentContent[MAX_TOKEN_SIZE];
  int currentContentSize = 0;
  TreeNode *result = malloc(sizeof(TreeNode));
  TreeNode *currentNode = result;
  int branchesStackSize = 16;
  TreeNode **branchesStack = calloc(branchesStackSize, sizeof(TreeNode));
  int openCount = 0;
  int firstVisits = 0;
  while ((character = igetc(i)) != EOF) {
    printf("char = %s\n", &character);
    if (character == ' ' && firstVisits < openCount) {
      TreeNode *newNode = malloc(sizeof(TreeNode));
      newNode->token.content = malloc(currentContentSize);
      strcpy(newNode->token.content, currentContent);
      newNode->branch = NULL;
      newNode->children = NULL;
      if (firstVisits < openCount) {
        if (firstVisits >= branchesStackSize) {
          TreeNode **oldStack = branchesStack;
          branchesStackSize *= 2;
          branchesStack = malloc(branchesStackSize);
          memcpy(branchesStack, oldStack, branchesStackSize / 2);
          free(oldStack);
        }
        branchesStack[firstVisits] = newNode;
        currentNode->branch = newNode;
        currentNode->branch->token.type = STATEMENT_FUNCTION;
        currentNode = currentNode->branch;
        firstVisits++;
        continue;
      }
      currentNode->children = newNode;
      currentNode->branch->token.type = getType(currentContent);
      currentNode = currentNode->children;
      continue;
    }
    if (character == '(' && openCount < 1) {
      openCount++;
      continue;
    }
    if (character == ')') {
      currentNode = branchesStack[firstVisits];
      firstVisits--;
      openCount--;
      continue;
    }
    if (currentContentSize < MAX_TOKEN_SIZE - 1) {
      currentContent[currentContentSize] = character;
      currentContent[currentContentSize + 1] = '\0';
      continue;
    }
    logError("parser error: token size limit exceeded.");
    return NULL;
  }
  return currentNode;
}
TreeNode *evaluate(TispyState ctx, TreeNode *root, char *funcName) {
  if (root == NULL) {
    return root;
  }
  char *content = root->token.content;
  TreeNode *evaluated = evaluate(ctx, root->children, content);
  if (root->token.type == STATEMENT_FUNCTION) {
    if (strcmp(content, "print")) {
    }
  } else {
    if (strcmp(funcName, "print")) {
    }
  }
  return evaluate(ctx, root->branch, funcName);
}
#endif

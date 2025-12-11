#ifndef TINY_LISP
#define TINY_LISP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKEN_SIZE 2048
#define MAX_STACK_SIZE 16 * 16 * 16 * 16
typedef enum TokenType { STATEMENT = 1, EXPRESSION } TokenType;
typedef struct Token {
  TokenType type;
  char *content;
} Token;
typedef struct TreeNode {
  struct TreeNode *children;
  struct TreeNode *branch;
  Token token;
} TreeNode;
static void logError(char *content) { printf("[ERROR]:\n%s\n", content); }
TreeNode *parse(FILE *file) {
  char character = ' ';
  char currentContent[MAX_TOKEN_SIZE];
  int currentContentSize = 0;
  TreeNode *result = malloc(sizeof(TreeNode));
  TreeNode *branchesStack[MAX_STACK_SIZE];
  TreeNode *currentNode = result;
  int openCount = 0;
  int firstVisits = 0;
  branchesStack[0] = result;
  while ((character = fgetc(file)) != EOF) {
    printf("char = %s\n", &character);
    if (character == ' ' && firstVisits < openCount) {
      TreeNode *newNode = malloc(sizeof(TreeNode));
      newNode->token.content = malloc(currentContentSize);
      strcpy(newNode->token.content, currentContent);
      newNode->branch = NULL;
      newNode->children = NULL;
      if (firstVisits < openCount) {
        if (firstVisits >= MAX_STACK_SIZE) {
          logError("parse error: theoretical stack size limit exceeded.");
          return NULL;
        }
        branchesStack[firstVisits] = newNode;
        currentNode->branch = newNode;
        currentNode->branch->token.type = STATEMENT;
        currentNode = currentNode->branch;
        firstVisits++;
        continue;
      }
      currentNode->children = newNode;
      currentNode->branch->token.type = EXPRESSION;
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
TreeNode *evaluate(TreeNode *root) {
  if (root == NULL) {
    return root;
  }
  TreeNode *evaluated = evaluate(root->children);
  if (root->token.type == STATEMENT) {
    if (strcmp(root->token.content, "car")) {
      return root;
    } else if (strcmp(root->token.content, ))
  }
  return evaluate(root->branch);
}
#endif

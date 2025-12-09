#ifndef TINY_LISP
#define TINY_LISP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TOKEN_SIZE 2048
#define MAX_STACK_SIZE 1024
typedef struct TreeNode {
  struct TreeNode *children;
  struct TreeNode *branch;
  char *content;
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
      newNode->content = malloc(currentContentSize);
      strcpy(newNode->content, currentContent);
      newNode->branch = NULL;
      newNode->children = NULL;
      if (firstVisits < openCount) {
        if (firstVisits >= MAX_STACK_SIZE) {
          logError("stack size limit exceeded.");
          return NULL;
        }
        branchesStack[firstVisits] = newNode;
        currentNode->branch = newNode;
        currentNode = currentNode->branch;
        firstVisits++;
        continue;
      }
      currentNode->children = newNode;
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
    logError("token size limit exceeded.");
    return NULL;
  }
  return currentNode;
}
int evaluate(TreeNode *root) {
  if (root == NULL) {
    logError("could not evaluate parse tree.");
    return 1;
  }
  printf("evaluating...");
  while (root != NULL) {
    TreeNode *child = root->children;
    if (strcmp(root->content, "print")) {
      while (child != NULL) {
        printf("%s\n", child->content);
        child = child->children;
      }
    }
    root = root->branch;
  }
  return 0;
}
#endif

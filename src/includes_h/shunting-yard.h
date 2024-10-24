#include "lexer.h"
typedef struct Node {
    char *value;
    TokenType type;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(char *value);

Node* createAstFromTokens(Node* firstNode, Token* token);

Node* freeAllNodes(Node* firstNode);
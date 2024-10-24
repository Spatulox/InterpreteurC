#include "includes.h"
#include "shunting-yard.h"

Node* createNode(char *value) {
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->type = DEFAULT;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* freeAllNodes(Node* firstNode) {
    if(firstNode->left){
        freeAllNodes(firstNode->left);
    }
    if(firstNode->right){
        freeAllNodes(firstNode->right);
    }
    free(firstNode);
    return NULL;
}

Node* createAstFromTokens(Node* firstNode, Token* token) {
    if(token == NULL){

        return firstNode;
    }
    if(firstNode == NULL){
        return createAstFromTokens(createNode(token->value), token->nextToken);
    }
    if(firstNode->left == NULL){
        firstNode->left = createAstFromTokens(createNode(token->value), token->nextToken);
    } else {
        firstNode->right = createAstFromTokens(createNode(token->value), token->nextToken);
    }
    return firstNode;
}
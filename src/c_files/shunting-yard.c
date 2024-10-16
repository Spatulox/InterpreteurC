#include "includes.h"
#include "shunting-yard.h"

Ast* createAst(char *value) {
    Ast* ast = malloc(sizeof(Ast));
    ast->value = value;
    ast->left = NULL;
    ast->right = NULL;
    return ast;
}
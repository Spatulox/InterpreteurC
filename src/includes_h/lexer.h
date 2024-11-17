//
// Created by Nab on 10/10/2024.
//

#ifndef LEXER_H
#define LEXER_H
#define MAX_TOKEN_SIZE 100

typedef enum TokenType{
    DEFAULT,
    NUMBER,
    OPERATOR,
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE,
    SCOPE_OPEN,
    SCOPE_CLOSE,
    HOOK_OPEN,
    HOOK_CLOSE,
    COMMA,
    ASSIGNMENT,
    VARIABLE,
    STRING_TOKEN,
    PRINT,
    SHOW_AST
} TokenType;
typedef struct Token {
    TokenType type;
    char *value;
    struct Token *nextToken;
} Token;
Token* createToken(const TokenType type, const char* value);
Token* freeAllTokens(Token* firstToken);
Token* lexerCalculator(char* input);
void printLexer(Token* firstToken);
#endif //LEXER_H

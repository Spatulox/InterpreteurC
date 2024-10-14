typedef enum TokenType {
    NUMBER,
    OPERATOR,
    PARENTHESIS,
    ASSIGNMENT,
    VARIABLE,
}TokenType;
typedef struct Ast {
    char *value;
    TokenType type;
    struct Ast *left;
    struct Ast *right;
} Ast;

Ast* createAst(char *value);
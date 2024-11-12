#include "includes.h"
#include "log.h"
#include "file.h"
#include "ast.h"
#include "lexer.h"


#define MAX_INSTRUCTION_SIZE 1000

void interpret(char *input);

// ------------------------------------------------------------------------ //

int readAndExecuteInstructionFile(const char *fileName){
    Log("INFO : Reading instructions file");
    int rows;
    int columns;
    char ** array = readInstructionFile(fileName, &rows, &columns);
    //printf("rows:%d\n", rows);
    for (int i = 0; i < rows; ++i) {
        printf("%s\n", array[i]);
    }

    // Avoid the compilator message with the useless "if"
    if(rows == 5){
        return -1;
    }
    return 0;
}

// ------------------------------------------------------------------------ //

//int executingUserInstructions(Node** ast, char* instruction) {
//    Log("[DEBUG] Processing user instruction");
//    Token* tokens = lexerCalculator(instruction);
//    if (tokens) {
//        *ast = createAstFromTokens(*ast, tokens);
//        *ast = freeAllNodes(*ast);
//        tokens = freeAllTokens(tokens);
//    }
//
//    return 0;
//}


// ------------------------------------------------------------------------ //


/*int main(){
    test_all();
    return 0;
}*/


int main() {
        Token *tokens = lexerCalculator("((113000000 + 3) * 2)");
        ASTNode *ast = parse_expression(&tokens);
        printf("Result: %d\n", eval(ast));

        return 0;
}

void interpret(char *input) {
    Token *tokens = lexerCalculator(input);
    Token *current = tokens;
    ASTNode *ast = parse_expression(&current);

    if (ast) eval(ast);
}

//int main(int argc, char **argv) {
//
//    if (argc >= 2) {
//        Log("Reading instructions");
//        const char* fileString = argv[1];
//        readAndExecuteInstructionFile(fileString);
//        return 0;
//    }
//
//    Node* ast = NULL;
//    Log("Manually enter instructions :");
//
//    char instruction[MAX_INSTRUCTION_SIZE];
//    int emptyLineCount = 0;
//
//    while (emptyLineCount < 2) {
//        printf("-> ");
//        // Get all the line
//        fflush(stdin);
//        if (fgets(instruction, sizeof(instruction), stdin) == NULL) {
//            break;
//        }
//
//        // Replace the last char by the "end of line" char
//        size_t len = strlen(instruction);
//        if (len > 0 && instruction[len-1] == '\n') {
//            instruction[len-1] = '\0';
//            len--;
//        }
//
//        if (len == 0) {
//            // Used to break the program if
//            emptyLineCount++;
//            printf("\n");
//        } else {
//            emptyLineCount = 0;
//            executingUserInstructions(&ast, instruction);
//        }
//    }
//
//    Log("INFO : Stopping program");
//    return 0;
//}
#include "includes.h"
#include "log.h"
#include "file.h"
#include "shunting-yard.h"
#include "lexer.h"

// ------------------------------------------------------------------------ //

int readAndExecuteInstructionFile(){
    Log("INFO : Reading instructions file");
    int rows;
    int columns;
    char ** array = readInstructionFile(&rows, &columns);
    //printf("rows:%d\n", rows);
    for (int i = 0; i < rows; ++i) {
        // executeInstruction(array[i]);
        printf("%s\n", array[i]);
    }

    // Avoid the compilator message with the useless "if"
    if(rows == 5){
        return -1;
    }
    return 0;
}


// ------------------------------------------------------------------------ //

int askingUserForInstructions() {
    Log("INFO : Asking user for instructions");

    printf("-> ");
    char instruction[MAX_TOKEN_SIZE -1];
    scanf("%s", instruction);
    Token* tokens = lexerCalculator(instruction);
    if(tokens){
        tokens = freeAllTokens(tokens);
    }
    
    return 0;
}



// ------------------------------------------------------------------------ //

int main() {
    char mainMenu = '0';
    do {
        printf("What do you want to do ?\n- 1 : Read the instructions file\n- 2 : Manually enter the instructions\n- 0 : Quit\n");
        fflush(stdin);
        scanf("%s", &mainMenu);

        switch (mainMenu) {
            case '0':
                break;

            case '1':
                // Read the file
                readAndExecuteInstructionFile();
                break;

            case '2':
                // Ask the user for the instructions
                askingUserForInstructions();
                break;

            default:
                mainMenu = '0';
                break;
        }

    }while(mainMenu != '0');

    Log("INFO : Stopping program\n");
    return 0;
}

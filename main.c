#include <stdio.h>
#include <stdlib.h>

#include "./src/includes_h/log.h"
#include "./src/includes_h/file.h"
#include "./src/includes_h/structs.h"

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

void executeInstruction(char * instruction){
    printf("%s", instruction);
}
// ------------------------------------------------------------------------ //

int askingUserForInstructions() {
    Log("INFO : Asking user for instructions");

    char userInput[1000];
    fflush(stdin);
    scanf("%s", userInput);

    ListVariable * varStorage = malloc(sizeof (Variable) * 1);


    if(varStorage == NULL){
        Log("Impossible to allocate memory for varStorage");
        return -1;
    }

    //lexer()
    //parse()
    //scanf
    //executeInstruction(var);

    executeInstruction(userInput);

    Log("Lecture terminé");
    return 0;
}

void test_createVariableNode();
void test_addVariableToList();

// ------------------------------------------------------------------------ //


/*int main(){
    test_createVariableNode();
    test_addVariableToList();
    return 0;
}*/

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
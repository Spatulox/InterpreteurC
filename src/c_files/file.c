//
// Created by Marc on 01/10/2024.
//
#include "includes.h"
#include "file.h"
#include "log.h"

void freeDoubleArray(void **array, int lines){
    for (int j = 0; j < lines; j++) {
        free(array[j]);
    }
    free(array);
    array = NULL;
}

// ------------------------------------------------------------------------ //
/**
 * Function which send the number of line of the instructions.txt file
 * @return
 */
int countLines(FILE * fp){
    //FILE *fp = fopen("instructions.txt", "rb");
    if(fp == NULL){
        printf("ERROR : Impossible d'ouvrir le fichier");
        return -1;
    }
    int count = 0;
    int var = fgetc(fp);
    while ( var != EOF){
        if(var == '\n'){
            count ++;
        }
        var = fgetc(fp);
    }
    // Last line is not count inside the loop;
    return ++count;
}

// ------------------------------------------------------------------------ //
/**
 * Function which send back the number of char of one line (start at line 1) of the instructions.txt file
 * @param line
 * @return
 */
int countCharInLine(FILE *fp, int line){
    if(fp == NULL){
        printf("ERROR : Impossible d'ouvrir le fichier");
        return -1;
    }
    rewind(fp);

    // Jump to the right line
    char var = fgetc(fp);
    for (int i = 0; i < line; i++) {
        //printf("Skipping line %d : ", i);
        while (var != '\n'){
            if(var == EOF){
                Log("ERROR : Unexpected end of file :/");
                return -1;
            }
            var = fgetc(fp);
        }
        var = fgetc(fp); // Pour incrémenter si je reste dans la boucle
    }
    if(fseek(fp, -1, SEEK_CUR) != 0){
        Log("ERROR : Impossible to move the cursor inside the file");
        return -1;
    }

    // Count les charactères de la ligne
    int count = 0;
    int ch = fgetc(fp);
    while (ch != EOF && ch != '\n' && ch != '\r' && ch != '\0') {
        ch = fgetc(fp);
        count++;
    }

    if (ch == EOF && count == 0) {
        Log("ERROR : Empty line or unexpected end of file");
        return -1;
    }

    return count;
}

// ------------------------------------------------------------------------ //

int skipToEndLineChars(FILE * fp){
    char var = fgetc(fp);
    while (var != EOF && (var == '\n' || var == '\r' || var == '\0')) {
        var = fgetc(fp);
    }

    if (var != EOF) {
        if (fseek(fp, -1, SEEK_CUR) != 0) {
            Log("ERROR : Problème lors du recul du curseur");
            return -1;
        }
    }

    if(var == EOF){
        return EOF;
    }
    return 0;
}

// ------------------------------------------------------------------------ //

char **readInstructionFile(const char *fileName, int *rows, int *columns) {

    int osbacktrack;
    #ifdef _WIN32
        osbacktrack = 2;
    #else
        osbacktrack = 1;
    #endif


    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        Log("ERROR : Impossible to open the file provided");
        return NULL;
    }

    int lines = countLines(fp);
    //printf("lines: %d\n", lines);
    if (lines <= 0) {
        Log("ERROR : File is empty or couldn't count lines");
        fclose(fp);
        return NULL;
    }

    *rows = lines;
    char **array = calloc(lines, sizeof(char *));
    if (array == NULL) {
        Log("ERROR : Memory allocation failed for rows");
        fclose(fp);
        return NULL;
    }

    rewind(fp);
    *columns = 0;

    for (int i = 0; i < lines; i++) {
        // Skip to the beginning of the next line

        //printf("----------------------\n");
        int charInLine = countCharInLine(fp, i); // Always count from current position
        //printf("Line %d: %d characters\n", i + 1, charInLine);
        //printf("----------------------\n");

        //printf("charInLine : %d\n", charInLine);
        // Sometimes the compilator do some shitty things here and say "Condition is always true", which is false :/
        if (charInLine == -1) {
            Log("ERROR : Impossible to count the char in the line");
            freeDoubleArray((void **)array, i);
            fclose(fp);
            return NULL;
        }


        if (charInLine > *columns) {
            *columns = charInLine;
        }

        array[i] = malloc((charInLine + 2) * sizeof(char)); // +2 for '\n' and '\0'
        if (array[i] == NULL) {
            Log("ERROR : Memory allocation failed for columns");
            freeDoubleArray((void **)array, i);
            fclose(fp);
            return NULL;
        }

        skipToEndLineChars(fp);
        // Read the line

        if (feof(fp)) {
            // Si on est à la fin du fichier, on recule de charInLine
            if (fseek(fp, -charInLine, SEEK_CUR) != 0) {
                Log("ERROR : Failed to set file position");
                freeDoubleArray((void **)array, i + 1);
                fclose(fp);
                return NULL;
            }
        } else {
            // Sinon, on recule de charInLine  + 1 (Linux) ou + 2 (Windows) pour les \r et \n
            if (fseek(fp, -charInLine - osbacktrack, SEEK_CUR) != 0) {
                Log("ERROR : Failed to set file position");
                freeDoubleArray((void **)array, i + 1);
                fclose(fp);
                return NULL;
            }
        }

        if (fgets(array[i], charInLine + osbacktrack, fp) == NULL) {
            Log("ERROR : Failed to read line from file");
            freeDoubleArray((void **)array, i + 1);
            fclose(fp);
            return NULL;
        }
        array[i][strcspn(array[i], "\r\n")] = '\0';
    }

    fclose(fp);
    return array;
}
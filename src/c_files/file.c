//
// Created by M78st on 01/10/2024.
//
#include <stdio.h>
#include <string.h>

#include "../includes_h/file.h"
#include "../includes_h/log.h"

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
    rewind(fp);
    if(fp == NULL){
        printf("ERROR : Impossible d'ouvrir le fichier");
        return -1;
    }

    // Jump to the right line
    char var = fgetc(fp);
    for (int i = 0; i < line; i++) {
        printf("Skipping line %d : ", i);
        while (var != '\n'){
            printf("%c", var);
            var = fgetc(fp);
        }
        printf("\n");
        var = fgetc(fp); // Pour incrémenter si je reste dans la boucle
    }
    fseek(fp, -1, SEEK_CUR);

    // Count les charactères de la ligne
    int count = 0;
    int ch = fgetc(fp);
    while (ch != EOF && ch != '\n' && ch != '\r' && ch != '\0') {
        ch = fgetc(fp);
        count++;
    }

    return count;
}

// ------------------------------------------------------------------------ //

int skipToEndLineChars(FILE * fp){
    char var = fgetc(fp);
    while (var != EOF && (var == '\n' || var == '\r' || var == '\0')) {
        var = fgetc(fp);
        //While char != \n || \r || \0 => avance le cursor
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

char **readInstructionFile(int *rows, int *columns) {
    FILE *fp = fopen("instructions.txt", "r");
    if (fp == NULL) {
        Log("ERROR : Impossible to open the 'instructions' file");
        return NULL;
    }

    int lines = countLines(fp);
    printf("lines: %d\n", lines);
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

        printf("----------------------\n");
        int charInLine = countCharInLine(fp, i); // Always count from current position
        printf("Line %d: %d characters\n", i + 1, charInLine);
        printf("----------------------\n");

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
        printf("Ftell avant error %ld\n", ftell(fp));

        if (feof(fp)) {
            // Si on est à la fin du fichier, on recule de charInLine
            if (fseek(fp, -charInLine, SEEK_CUR) != 0) {
                Log("ERROR : Failed to set file position");
                freeDoubleArray((void **)array, i + 1);
                fclose(fp);
                return NULL;
            }
        } else {
            // Sinon, on recule de charInLine + 2 pour les \r et \n
            if (fseek(fp, -charInLine - 2, SEEK_CUR) != 0) {
                Log("ERROR : Failed to set file position");
                freeDoubleArray((void **)array, i + 1);
                fclose(fp);
                return NULL;
            }
        }

        printf("Ftell2 avant error %ld\n", ftell(fp));
        if (fgets(array[i], charInLine + 2, fp) == NULL) {
            Log("ERROR : Failed to read line from file");
            freeDoubleArray((void **)array, i + 1);
            fclose(fp);
            return NULL;
        }

        // Retire les \r et \n de fin de ligne et met un \0
        size_t len = strlen(array[i]);
        if (len > 0 && (array[i][len - 1] == '\n' || array[i][len - 1] == '\r')) {
            array[i][len - 1] = '\0';
        }
    }

    fclose(fp);
    return array;
}
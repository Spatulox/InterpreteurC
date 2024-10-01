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
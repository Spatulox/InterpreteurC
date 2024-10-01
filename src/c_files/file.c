//
// Created by M78st on 01/10/2024.
//
#include <stdio.h>
#include "../includes_h/file.h"

// ------------------------------------------------------------------------ //
/**
 * Function which send the number of line of the instructions.txt file
 * @return
 */
int countLines(){
    FILE *fp = fopen("instructions.txt", "rb");
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
    fclose(fp);
    // Last line is not count inside the loop;
    return ++count;
}

// ------------------------------------------------------------------------ //
/**
 * Function which send back the number of char of one line (start at line 1) of the instructions.txt file
 * @param line
 * @return
 */
int countCharInLine(int line){
    if(line == 0){
        Log("ERROR : You need to give a valid line ( 1+ )");
        return -1;
    }
    FILE *fp = fopen("instructions.txt", "r");
    if(fp == NULL){
        printf("ERROR : Impossible d'ouvrir le fichier");
        return -1;
    }
    char buff[1000];
    // lines - 1 because the file start at line 1 and the user want to start at line 1;
    for (int i = 0; i < line-1; ++i) {
        if (fgets(buff, 1000, fp) == NULL) {
            fclose(fp);
            return -1;  // La ligne demandée n'existe pas
        }
    }

    int count = 0;
    int var = fgetc(fp);
    while ((var = fgetc(fp)) != EOF && var != '\n' && var != '\r'){ // Retour chariot de windows
        count ++;
    }
    fclose(fp);

    // Last line is not count inside the loop;
    return ++count;
}
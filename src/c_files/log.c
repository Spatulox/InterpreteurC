//
// Created by Marc on 01/10/2024.
//
#include "includes.h"
#include "log.h"

void Log(char *string){
    // Create the time string
    time_t t = time(NULL);

    struct tm* tm;
    tm = localtime(&t);
    char date[20];
    strftime(date, 20, "%d/%m/%Y %H:%M:%S", tm);


    fflush(stdout);

    // Write the file
    FILE *file = fopen("log.txt", "a+");
    if (file == NULL) {
        printf("[%s] : ERROR: Unable to open the log file\n", date);
        return;
    }
    fprintf(file, "[%s] : %s\n", date, string);
    fclose(file);
    fflush(stdout);

    printf("[%s] : %s\n", date, string);
}
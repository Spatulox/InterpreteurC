//
// Created by M78st on 01/10/2024.
//

#ifndef INTERPRETEURC_FILE_H
#define INTERPRETEURC_FILE_H

int countLines(FILE * fp);
int countCharInLine(FILE *fp, int line);
int skipToEndLineChars(FILE * fp);
char **readInstructionFile(const char *fileName, int *rows, int *columns);

#endif //INTERPRETEURC_FILE_H

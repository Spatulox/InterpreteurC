//
// Created by M78st on 14/11/2024.
//

#ifndef INTERPRETEURC_STRINGUTILS_H
#define INTERPRETEURC_STRINGUTILS_H

#include "ast.h"

number tryConvertToNumber(number v);
int castStringIntoNumber(number *left, number *right);
void castNumberIntoString(number astNumber, char **resultString);
#endif //INTERPRETEURC_STRINGUTILS_H

/*////////////////////////////////////////////////////////*/
//                                                        //   
// Name: parser.h                                     //                     
// Author: Enzo M                                         //  
// Date: 15/11/2024                                          //  
// Description: ...                                       //
//                                                        //  
/*////////////////////////////////////////////////////////*/

#ifndef INTERPRETEURC_PARSER_H
#define INTERPRETEURC_PARSER_H

#include "includes.h"
#include "structs.h"
#include "ast.h"
#include "math.h"

ASTNode *create_number_node(const char* value);
ASTNode *create_string_node(Token **tokens);
ASTNode *create_binary_op_node(ASTNode *left, ASTNode *right, char op);
ASTNode* parse_expression(Token **tokens);
ASTNode* parse_term(Token **tokens);
ASTNode* parse_primary(Token **tokens);
ASTNode* parse_assignment(Token **tokens);
void showAst(ASTNode *node, int level, const char* prefix);
#endif //INTERPRETEURC_PARSER_H

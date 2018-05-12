#pragma once

#include "uthash.h"
#include "types.h"
#include "symboltable.h"
#include "logger.h"
#include "typecheck.h"
#include <stdlib.h>
#include <stdio.h>

FILE *ir;

expression_t assign(int line, int col, expression_t exp1, expression_t exp2);
expression_t logicalOr(int line, int col, expression_t exp1, expression_t exp2);
expression_t logicalAnd(int line, int col, expression_t exp1, expression_t exp2);
expression_t logicalNot(int line, int col, expression_t exp);
expression_t equals(int line, int col, expression_t exp1, expression_t exp2);
expression_t notEquals(int line, int col, expression_t exp1, expression_t exp2);
expression_t lesser(int line, int col, expression_t exp1, expression_t exp2);
expression_t lesserEquals(int line, int col, expression_t exp1, expression_t exp2);
expression_t greaterEquals(int line, int col, expression_t exp1, expression_t exp2);
expression_t greater(int line, int col, expression_t exp1, expression_t exp2);
expression_t plus(int line, int col, expression_t exp1, expression_t exp2);
expression_t minus(int line, int col, expression_t exp1, expression_t exp2);
expression_t shiftLeft(int line, int col, expression_t exp1, expression_t exp2);
expression_t shiftRight(int line, int col, expression_t exp1, expression_t exp2);
expression_t multiply(int line, int col, expression_t exp1, expression_t exp2);
expression_t divide(int line, int col, expression_t exp1, expression_t exp2);
expression_t unaryMinus(int line, int col, expression_t exp);
expression_t unaryPlus(int line, int col, expression_t exp);


void eval(int line, int col, expression_t exp);
expression_t evalArray(int line, int col, expression_t exp);
expression_t evalFunc(int line, int col, expression_t exp);


expression_t twoExpHandler(int line, int col, expression_t exp1, expression_t exp2, char* operand);
char * oneExpHandler(expression_t exp1, char operand);
expression_t relOp(int line, int col, expression_t exp1, expression_t exp2, char* operand);
//void evalIf();
//void evalElse();
//void while
//call function
//define function
//declare function
//return

funcCallParamList_t* addExprAsParam(int line, int col, funcCallParamList_t *paramList, expression_t exp);

int ifStart(int line, int col, expression_t exp);
void ifEnd(int line, int col, int label);
void elseEnd(int line, int col, int label);
void whileEnd(int line, int col, int jump, int label);

int createJump();
void createMainJump();
int createLabel();
void createFunctionLabel(char *label);

void returnVoid(int line, int col);
void returnInt(int line, int col, expression_t exp);

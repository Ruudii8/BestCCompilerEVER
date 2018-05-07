

#pragma once

#include "uthash.h"
#include "types.h"
#include "symboltable.h"
#include "logger.h"


int checkVoid(int line, int col, int type);
int checkVar(int line, int col, char *name);
int checkFunc(int line, int col, char *name, int type, variable_t *parameters);
int checkFuncName(int line, int col, char *name);
int compareParams(variable_t *p1, variable_t *p2);


int checkForInt(int line, int col, expression_t exp);
int checkForArray(int line, int col, expression_t exp);
int checkIfAssignable(int line, int col, expression_t exp);
void checkReturnInt(int line, int col, expression_t exp);
void checkReturnVoid(int line, int col);
void checkFuncCallParams(int line, int col, expression_t exp);



expression_t* checkAssignment(int line, int col, expression_t *exp1, expression_t *exp2);

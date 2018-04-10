#pragma once

#include "uthash.h"
#include "types.h"
#include "symboltable.h"
#include "logger.h"


int checkVoid(int line, int col, int type);
int checkVar(int line, int col, char *name);
int checkFunc(int line, int col, char *name, int type, variable_t *parameters);
int checkFuncName(int line, int col, char *name);
void checkReturnInt(int line, int col, expression_t *exp);
void checkReturnVoid(int line, int col);


expression_t* checkAssignment(int line, int col, expression_t *exp1, expression_t *exp2);
#pragma once

#include "uthash.h"
#include "types.h"




symboltable_t symboltable;

variable_t *tmpParameters;
int tmpParameterCount;


extern void init(void);
extern void printAll(int line, int col);
extern void addVariable(int line, int col, var_tmp_t tmp, int type);
extern void declareFunction(int line, int col, char *name, int returnType, variable_t *parameters);
extern void defineFunction(int line, int col, char *name, int returnType, variable_t *parameters);
extern void endFunctionScope(int line, int col);
extern void addParameter(int line, int col, var_tmp_t tmp, int type);
extern variable_t* returnParameters(void);



extern void checkAssignment(int line, int col, char *name, char *name1);
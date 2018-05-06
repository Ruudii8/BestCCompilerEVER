#pragma once

#include "uthash.h"
#include "types.h"

symboltable_t symboltable;

variable_t *tmpParameters;
int tmpParameterCount;
   

void init(void);
void printAll(int line, int col);
void addVariable(int line, int col, var_tmp_t tmp, int type);
void declareFunction(int line, int col, char *name, int returnType, variable_t *parameters);
void defineFunction(int line, int col, char *name, int returnType, variable_t *parameters);
void endFunctionScope(int line, int col);
void addParameter(int line, int col, var_tmp_t tmp, int type);
variable_t* returnParameters(void);


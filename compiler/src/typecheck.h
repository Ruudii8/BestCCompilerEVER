#pragma once

#include "uthash.h"
#include "types.h"
#include "symboltable.h"


int checkVoid(int type);
int checkVar(variable_t *variables, char *name);
int checkFunc(function_t *functions, char *name);

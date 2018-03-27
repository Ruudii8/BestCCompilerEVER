#pragma once

#include "uthash.h"

#define TYPE_VOID 0
#define TYPE_INT 1


typedef struct 
{
    char *name;
    int type;
    int size;
    int order;              //only used for parameters
    UT_hash_handle hh;

}variable_t;


typedef struct 
{
    char *name;
    int returnType;
    int parameterCount;
    variable_t *parameters;
    int referenceLine;
    UT_hash_handle hh;

} function_t;


typedef struct 
{
    int id;
    variable_t *variables;
    UT_hash_handle hh;

} scope_t;


typedef struct 
{
    scope_t *scopes;
    function_t *functions;
    int scopeCounter;
    scope_t *currentScope;

} symboltable_t;


symboltable_t symboltable;

variable_t *tmpParameters;
int tmpParameterCount;


extern void init(void);
extern void printAll(int line, int col);
extern void addVariable(int line, int col, char *name, int type);
extern void declareFunction(int line, int col, char *name, int returnType, variable_t *parameters);
extern void defineFunction(int line, int col, char *name, int returnType, variable_t *parameters);
extern void endFunctionScope(int line, int col);
extern void addParameter(int line, int col, char *name, int type);
extern variable_t* returnParameters(void);
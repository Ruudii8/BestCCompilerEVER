#pragma once

#include "uthash.h"

#define TYPE_VOID 0
#define TYPE_INT 1
#define TYPE_INTARRAY 2

#define EXP_TYPE_LITERAL 0
#define EXP_TYPE_VAR 1
#define EXP_TYPE_ARR 2
#define EXP_TYPE_FUNC 3
#define EXP_TYPE_EXP 4
#define EXP_TYPE_TVALUE 5

//structs


typedef struct
{
    char *name;
    int size;

} var_tmp_t;


typedef struct 
{
    char *name;
    int type;
    int size;
    int order;              //only used for parameters
    UT_hash_handle hh;

} variable_t;


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
    function_t *currentFunction;

} symboltable_t;


struct exp
{
    int exp_type;
    int literal;
    char *var;
    struct funcCallParamList *paramList;
    struct exp *exp;

};

typedef struct exp expression_t;


struct funcCallParamList
{
    expression_t exp;
    struct funcCallParamList *prev;
    struct funcCallParamList *next;

};

typedef struct funcCallParamList funcCallParamList_t;

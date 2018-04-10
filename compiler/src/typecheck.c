#include "typecheck.h"
#include <stdio.h>
#include <types.h>

//here comes all the type testing stuff

//return type correct, correct zuweisungen


int checkVoid(int line, int col, int type)
{
   
    if(type==TYPE_VOID)
    {
        log.error(line, col, "A variable can not be void");
        return 1;
    }
    return 0;
}


int checkVar(int line, int col, char *name)
{
    variable_t *variable;
    HASH_FIND_STR(symboltable.currentScope->variables, name, variable);
    if(variable!=NULL)
    {
        log.error(line, col, "Variable with name %s already exists", name);
        return 1;
    }

    return 0;
}

int checkFunc(int line, int col, char *name, int type, variable_t *parameters)
{
    function_t *function;
    HASH_FIND_STR(symboltable.functions, name, function);
    if(function!=NULL)
    {
        if(function->referenceLine!=0)
        {
            log.error(line, col, "Function with name %s is already defined", name);
            return 1;
        }

        if(function->returnType!=type)
        {
            log.error(line, col, "Return type of definition of function %s does not match the function declaration", name);
            return 1;
        }

        if(compareParams(function->parameters, parameters))
        {
            log.error(line, col, "Parameters of definition of function %s do not match the function declaration", name);
            return 1;
        }
    }
    return 0;
}

int checkFuncName(int line, int col, char *name)
{
   function_t *function;
    HASH_FIND_STR(symboltable.functions, name, function);
    if(function!=NULL)
    {
        log.error(line, col, "Name %s is already used for a function", name);
        return 1;
    }

    return 0;
}



expression_t* checkAssignment(int line, int col, expression_t *exp1, expression_t *exp2)
{

     log.info(line, col, "exp1 = %d, exp2 = %d", exp1->exp_type, exp2->exp_type);


    if(exp1->exp_type == EXP_TYPE_LITERAL)
    {

        if(exp2->exp_type == EXP_TYPE_VAR)
        {
            variable_t *variable;
            HASH_FIND_STR(symboltable.currentScope->variables, exp2->var, variable);
            if(variable->type == TYPE_VOID)
            {
                log.error(line, col, "void cannot be assigned to int");
            }
            if(variable->type == TYPE_INTARRAY)
            {
                log.error(line, col, "int[] cannot be assigned to int");
            }
        }
    }


    expression_t *exp;

    return NULL;
   
}





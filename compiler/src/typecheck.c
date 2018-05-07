#include "typecheck.h"
#include <stdio.h>

//checks if type is void and throws and throws an error when true
int checkVoid(int line, int col, int type)
{
    if(type==TYPE_VOID)
    {
        log.error(line, col, "A variable can not be void");
        return 1;
    }
    return 0;
}

//Checks if var with same name already exists and throws an error wehn true
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

//Checks if function with same name already exists and whether its already declared.
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

        //When function already is declared but not defined paramters must be checked

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


        function->referenceLine = line;
    }


    return 0;
}

//Checks if function name already exists and throws an error wehn true
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





int checkForInt(int line, int col, expression_t exp)       //func TBD 
{
    if(exp.exp_type == EXP_TYPE_LITERAL)
    {
        return 1;
    }

    if(exp.exp_type == EXP_TYPE_ARR)
    {
        if(checkForInt(line, col, *exp.exp))
        {
            return 1;
        }
    }

    

    if(exp.exp_type == EXP_TYPE_VAR)
    {

        //if variable begins with . it's from intermediate code
        if(strcmp(exp.var, ".")){
            return 1;
        }

        variable_t *variable;
        HASH_FIND_STR(symboltable.currentScope->variables, exp.var, variable);
        if(variable!=NULL)
        {
            if(variable->type == TYPE_INT)
            {
                
                return 1;
            }
            
        }

        int id = 0;
        scope_t *scope;
        HASH_FIND_INT(symboltable.scopes, &id, scope);


        HASH_FIND_STR(scope->variables, exp.var, variable);
        if(variable!=NULL)
        {


            if(variable->type == TYPE_INT)
            {
                return 1;
            }
        }

    }

    if(exp.exp_type == EXP_TYPE_FUNC)      //TBD
    {
        function_t *function;
        HASH_FIND_STR(symboltable.functions, exp.var, function);
        if(function==NULL)
        {
            return 0;
        }
        if(function!=NULL)
        {
            if(function->returnType == TYPE_INT)
            {
                return 1;
            }
        }
    }



    log.error(line, col, "Expression must be type int");

    return 0;
}


int checkForArray(int line, int col, expression_t exp)
{
    if(exp.exp_type == EXP_TYPE_VAR)
    {
        variable_t *variable;
        HASH_FIND_STR(symboltable.currentScope->variables, exp.var, variable);
        if(variable!=NULL)
        {
            if(variable->type == TYPE_INTARRAY)
            {
                return 1;
            }
        }

        int id = 0;
        scope_t *scope;
        HASH_FIND_INT(symboltable.scopes, &id, scope);


        HASH_FIND_STR(scope->variables, exp.var, variable);
        if(variable!=NULL)
        {
            if(variable->type == TYPE_INTARRAY)
            {
                return 1;
            }
        }

    }

    log.error(line, col, "Expression must be type array");

    return 0;
}


int checkIfAssignable(int line, int col, expression_t exp)
{



    if(exp.exp_type == EXP_TYPE_ARR)
    {
        if(checkForInt(line, col, *exp.exp))
        {
            return 1;
        }

    }

    if(exp.exp_type == EXP_TYPE_VAR)
    {


        variable_t *variable;
        HASH_FIND_STR(symboltable.currentScope->variables, exp.var, variable);
        if(variable!=NULL)
        {

            if(variable->type == TYPE_INT)
            {
                return 1;
            }
        }


        int id = 0;
        scope_t *scope;
        HASH_FIND_INT(symboltable.scopes, &id, scope);


        HASH_FIND_STR(scope->variables, exp.var, variable);
        if(variable!=NULL)
        {


            if(variable->type == TYPE_INT)
            {
                return 1;
            }
        }

    }

    log.error(line, col, "Expression must be assignable");

    return 0;

}

void checkReturnInt(int line, int col, expression_t exp)
{
    //TODO check if exp is an int
    if(!checkForInt(line, col, exp))
    {

    }



    if(symboltable.currentFunction->returnType != TYPE_INT)
    {
        log.error(line, col, "Return value does not match return type");

    }

    return;
}


void checkReturnVoid(int line, int col)
{
    if(symboltable.currentFunction->returnType != TYPE_VOID)
    {
        log.error(line, col, "Return value does not match return type");
    }

    return;
}


void checkFuncCallParams(int line, int col, expression_t exp)
{
    function_t *function;
    HASH_FIND_STR(symboltable.functions, exp.var, function);
    if(function != NULL)
    {
        funcCallParamList_t *paramList = (funcCallParamList_t*)malloc(sizeof(funcCallParamList_t));

        paramList = exp.paramList;

        if(paramList != NULL)
        {
            while(paramList->prev != NULL)
            {
                paramList = paramList->prev;
            }
        }
       

        variable_t *param, *tmpVar;

        HASH_ITER(hh, function->parameters, param, tmpVar)
        {
            if(paramList == NULL)
            {
                log.error(line, col, "Missing parameters on function call");
                return;
            }


            if(param->type == TYPE_INT)
            {
                if(!checkForInt(line, col, paramList->exp))
                {
                    
                    return;
                }
            }

            if(param->type == TYPE_INTARRAY)
            {
               
                if(!checkForArray(line, col, paramList->exp))
                {
                    return;
                }
            }

            paramList = paramList->next;

        }

        if(paramList != NULL)
        {
            log.error(line, col, "Too many parameters on function call");
        }

    }
    else
    {
        log.error(line, col, "Called function does not exist");
    }

    return;
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

    if(exp1->exp_type == EXP_TYPE_VAR)
    {
        variable_t *variable;
        HASH_FIND_STR(symboltable.currentScope->variables, exp1->var, variable);
        if(variable->type == TYPE_INTARRAY)
        {
            if(exp2 != NULL)
            {
                log.error(line, col, "nothing can be assigned to an int[]");
            }
        }   
        if(variable->type == TYPE_VOID)
        {
            log.error(line, col, "no variable with type void allowed");
        }
    }

    




    expression_t *exp;

    return NULL;
   
}

//compares if type of params is equal
int compareParams(variable_t *p1, variable_t *p2)
{
    variable_t *params1, *tmpVar, *params2;
    HASH_ITER(hh, p1, params1, tmpVar)
    {

        HASH_FIND_STR(p2, params1->name, params2);
        if(params2 == NULL)
        {
            return 1;
        }

        if(params1->type != params2->type || params1->order != params2->order)
        {
            return 1;
        }
    }

    if(params2->hh.next != NULL)
    {
        return 1;
    }

    return 0;
}




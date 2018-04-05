#include <stdio.h> 
#include <string.h>
#include "uthash.h"

#include "symboltable.h"
#include "diag.h"
#include "logger.h"


variable_t *tmpParameters = NULL;

void init()
{
    symboltable.scopeCounter = 1;
    symboltable.scopes = NULL;
    symboltable.functions = NULL;

    scope_t *global_scope = (scope_t*) malloc(sizeof(scope_t));

    global_scope->id = 0;
    global_scope->variables = NULL;

    symboltable.currentScope = global_scope;

    HASH_ADD_INT(symboltable.scopes, id, global_scope);

    tmpParameterCount = 0;

}

char* returnTypeString(int type)
{
    if(type == 1)
    {
        return "int";
    }
    else if(type==0)
    {
        return "void";
    }
    else
    {
        return "int[]";
    }
}


void printAll(int line, int col)
{
    // print symbol tables

    printf("\n################################### Symbol tables ####################################");
    
    printf("\nVariables:");


    scope_t *scope, *tmpScope;
    HASH_ITER(hh, symboltable.scopes, scope, tmpScope)
    {
        printf("\nScope: %d\n", scope->id);
        printf("-------------------------------------------\n");
        printf("| Name       | Type  | Size   | Offset    |\n");
        printf("|-----------------------------------------|\n");

        variable_t *variable, *tmpVar;
        HASH_ITER(hh, scope->variables, variable, tmpVar)
        {
            printf("|%12s|%7s|%8d|           |\n", variable->name, returnTypeString(variable->type), variable->size);
        }

        printf("-------------------------------------------\n");
    }



    printf("\nFunctions:\n");

    printf("--------------------------------------------------------------------------------------\n");
    printf("| Name       | Returntype | ParamCount | Reference  | Parameters                     |\n");
    printf("|------------------------------------------------------------------------------------|\n");


    function_t *function, *tmpFunc;
    HASH_ITER(hh, symboltable.functions, function, tmpFunc)
    {
        printf("|%12s|%12s|%12d|            | ", function->name, returnTypeString(function->returnType), function->parameterCount);

        variable_t *parameter, *tmpPar;
        if(function->parameters!=NULL){
            HASH_ITER(hh, function->parameters, parameter, tmpPar)
            {
                if(parameter->type == 2)
                {
                    printf("int %s[%d]", parameter->name, parameter->size/4);
                }
                else
                {
                    printf("%s %s", returnTypeString(parameter->type), parameter->name);
                }


                if(tmpPar==NULL)
                {
                    printf("\n");
                }
                else
                {
                    printf(", ");
                    
                }
            }
        }
        else
        {
            printf("\n");
        }
    }

    printf("--------------------------------------------------------------------------------------\n");


    printf("\n################################### Symbol tables ####################################\n");

}


void addVariable(int line, int col, var_tmp_t tmp, int type)
{
    //check if type is void

    if(type==0)
    {
        log.error(line, col, "A variable can not be void");
        return;
    }

    //check if variable with same name already exists

    variable_t *variable;
    HASH_FIND_STR(symboltable.currentScope->variables, tmp.name, variable);
    if(variable!=NULL)
    {
        log.error(line, col, "Variable with name %s already exists", tmp.name);
        return;
    }

    //check if function with same name already exists

    function_t *function;
    HASH_FIND_STR(symboltable.functions, tmp.name, function);
    if(function!=NULL)
    {
        log.error(line, col, "Name %s is already used for a function", tmp.name);
        return;
    }

    //add new variable

    variable = (variable_t*)malloc(sizeof(variable_t));

    variable->name = tmp.name;
    if(tmp.size==-1)
    {
        variable->size = 4;
        variable->type = type;
    }
    else{
        variable->size = tmp.size*4;
        variable->type = TYPE_INTARRAY;
    }


    HASH_ADD_STR(symboltable.currentScope->variables, name, variable);

}

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


void declareFunction(int line, int col, char *name, int returnType, variable_t *parameters)
{
    //check if variable with same name already exists

    variable_t *variable;
    HASH_FIND_STR(symboltable.currentScope->variables, name, variable);
    if(variable!=NULL)
    {
        log.error(line, col, "Name %s is already used for a variable", name);
        return;
    }
    
    //check if function with same name exists already

    function_t *function;
    HASH_FIND_STR(symboltable.functions, name, function);
    if(function!=NULL)
    {
        log.error(line, col, "Function with name %s is already declared", name);
        return;
    }

    //add new function

    function = (function_t*)malloc(sizeof(function_t));

    function->name = name;
    function->returnType = returnType;
    function->parameters = parameters;
    function->referenceLine = 0;

    if(parameters==NULL)
    {
        function->parameterCount = 0;
    }
    else
    {
        function->parameterCount = tmpParameterCount;
        tmpParameterCount = 0;
    }
            
    HASH_ADD_STR(symboltable.functions, name, function);

}


void defineFunction(int line, int col, char *name, int returnType, variable_t *parameters)
{
    //check if variable with same name already exists

    variable_t *variable;
    HASH_FIND_STR(symboltable.currentScope->variables, name, variable);
    if(variable!=NULL)
    {
        log.error(line, col, "Name %s is already used for a variable", name);
        return;
    }
    
    //check if function with same name exists already

    function_t *function;
    HASH_FIND_STR(symboltable.functions, name, function);
    if(function!=NULL)
    {

        //check whether function is already defined

        if(function->referenceLine!=0)
        {
            log.error(line, col, "Function with name %s is already defined", name);
            return;
        }

        if(function->returnType!=returnType)
        {
            log.error(line, col, "Return type of definition of function %s does not match the function declaration", name);
            return;
        }

        if(compareParams(function->parameters, parameters))
        {
            log.error(line, col, "Parameters of definition of function %s do not match the function declaration", name);
            return;
        }

        //add function definition
        function->referenceLine = line;
        
    }
    else
    {
        //add new function

        function = (function_t*)malloc(sizeof(function_t));

        function->name = name;
        function->returnType = returnType;
        function->parameters = parameters;
        function->referenceLine = line;

        if(parameters==NULL)
        {
            function->parameterCount = 0;
        }
        else
        {
            function->parameterCount = tmpParameterCount;
            tmpParameterCount = 0;
        }
                
        HASH_ADD_STR(symboltable.functions, name, function);
    }
    
    scope_t *function_scope = (scope_t*) malloc(sizeof(scope_t));

    variable_t *tmpParams = parameters;

   

    function_scope->id = symboltable.scopeCounter;
    function_scope->variables = tmpParams;

    symboltable.currentScope = function_scope;
    symboltable.scopeCounter++;

    HASH_ADD_INT(symboltable.scopes, id, function_scope);

}


void addParameter(int line, int col, var_tmp_t tmp, int type)
{
    //check if variable with same name already exists

    variable_t *parameter;
    HASH_FIND_STR(tmpParameters, tmp.name, parameter);
    if(parameter!=NULL)
    {
        log.error(line, col, "Parameter with name %s already exists", tmp.name);
        return;
    }

    //add new parameter

    parameter = (variable_t*)malloc(sizeof(variable_t));

    parameter->name = tmp.name;

    if(tmp.size==-1)
    {
        parameter->size = 4;
        parameter->type = type;
    }
    else{
        parameter->size = tmp.size*4;
        parameter->type = TYPE_INTARRAY;
    }

    parameter->order = tmpParameterCount;
    tmpParameterCount++;

    HASH_ADD_STR(tmpParameters, name, parameter);

}


variable_t* returnParameters(void)
{
    variable_t *parameters = tmpParameters;
    tmpParameters = NULL;
    return parameters;
}


void endFunctionScope(int line, int col)
{
    int id = 0;
    scope_t *scope;
    HASH_FIND_INT(symboltable.scopes, &id, scope);
    symboltable.currentScope = scope;
}


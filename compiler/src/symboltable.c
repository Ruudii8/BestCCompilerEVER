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

/*char* parametersToString(variable_t *parameters)
{
    char *parameterString = "";

    if(parameters == NULL)
    {
        return "void";
    }

    variable_t *parameter, *tmpPar;
    HASH_ITER(hh, parameters, parameter, tmpPar)
    {
        strcat(parameterString, returnTypeString(parameter->type));
        strcat(parameterString, " ");
        strcat(parameterString, parameter->name);
        strcat(parameterString, ",");

    }

    parameterString[strlen(parameterString)-1] = '\0';


    //TODO

    return parameterString;

}*/

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
    printf("| Name       | Returntype | ParamCount | Parameters                      | Reference |\n");
    printf("|------------------------------------------------------------------------------------|\n");


    function_t *function, *tmpFunc;
    HASH_ITER(hh, symboltable.functions, function, tmpFunc)
    {
        printf("|%12s|%12s|%12d|", function->name, returnTypeString(function->returnType), function->parameterCount);

        variable_t *parameter, *tmpPar;
        if(function->parameters!=NULL){
            HASH_ITER(hh, function->parameters, parameter, tmpPar)
            {
                if(tmpPar==NULL)
                {
                    printf("%s %s\n",returnTypeString(parameter->type), parameter->name);
                }
                else
                {
                    printf("%s %s, ",returnTypeString(parameter->type), parameter->name);
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


void addVariable(int line, int col, id_def_t id_def, int type)
{
    //check if type is void

    if(type==0)
    {
        log.error(line, col, "A variable can not be void");
        return;
    }

    //check if variable with same name already exists

    variable_t *variable;
    HASH_FIND_STR(symboltable.currentScope->variables, id_def.name, variable);
    if(variable!=NULL)
    {
        log.error(line, col, "Variable with name %s already exists", id_def.name);
        return;
    }

    //check if function with same name already exists

    function_t *function;
    HASH_FIND_STR(symboltable.functions, id_def.name, function);
    if(function!=NULL)
    {
        log.error(line, col, "Name %s is already used for a function", id_def.name);
        return;
    }

    //add new variable

    variable = (variable_t*)malloc(sizeof(variable_t));

    variable->name = id_def.name;
    if(id_def.size==-1)
    {
        variable->size = 4;
        variable->type = type;
    }
    else{
        variable->size = id_def.size*4;
        variable->type = TYPE_INTARRAY;
    }


    HASH_ADD_STR(symboltable.currentScope->variables, name, variable);

}

int compareParams(variable_t *p1, variable_t *p2)
{

    /*for (p1; p1 != NULL; p1 = p1->hh.next, p2 = p2->hh.next)
    {
        if (p2 == NULL || p1->type != p2->type || p1->name != p2->name)
        {
            return 1;
        }
    }*/

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

    function_scope->id = symboltable.scopeCounter;
    function_scope->variables = function->parameters;

    symboltable.currentScope = function_scope;
    symboltable.scopeCounter++;

    HASH_ADD_INT(symboltable.scopes, id, function_scope);

}


void addParameter(int line, int col, id_def_t id_def, int type)
{
    //check if variable with same name already exists

    variable_t *parameter;
    HASH_FIND_STR(tmpParameters, id_def.name, parameter);
    if(parameter!=NULL)
    {
        log.error(line, col, "Parameter with name %s already exists", id_def.name);
        return;
    }

    //add new parameter

    parameter = (variable_t*)malloc(sizeof(variable_t));

    parameter->name = id_def.name;

    if(id_def.size==-1)
    {
        parameter->size = 4;
        parameter->type = type;
    }
    else{
        parameter->size = id_def.size*4;
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


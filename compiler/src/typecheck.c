#include "typecheck.h"
#include <stdio.h>


//here comes all the type testing stuff

//return type correct, correct zuweisungen


int checkVoid(int line, int col, int type)
{
    //TODO


    return 0;
}


int checkVar(int line, int col, char *name)
{
    //TODO


    return 0;
}


int checkFunc(int line, int col, char *name)
{
    //TODO

    symboltable.currentScope;

    return 0;
}



expression_t* checkAssignment(int line, int col, expression_t *exp1, expression_t *exp2)
{

     log.info(line, col, "exp1 = %d, exp2 = %d", exp1->exp_type, exp2->exp_type);




    if(exp2->exp_type == EXP_TYPE_ARR)
    {
        log.error(line, col, "error");
    }



    expression_t *exp;

    return NULL;
   
}



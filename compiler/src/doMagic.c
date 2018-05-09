#include "doMagic.h"

int counter = 0;

expression_t assign(int line, int col, expression_t exp1, expression_t exp2)
{
    checkIfAssignable(line, col, exp1);
    checkForInt(line, col, exp2);

    //if type is literal print value, else print name -- Array needs to be handled different
    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "aint %s = %d;\n", exp1.var, exp2.literal);
    }
    else if (exp2.exp_type == EXP_TYPE_VAR )
    {
        fprintf(ir, "aint %s = %s;\n", exp1.var, exp2.var);
    }

    
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t logicalOr(int line, int col, expression_t exp1, expression_t exp2)
{

    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    char *tmp = twoExpHandler(exp1, exp2, '||');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t logicalAnd(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    char *tmp = twoExpHandler(exp1, exp2, '&&');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t logicalNot(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);

    char *tmp = oneExpHandler(exp, '!');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t equals(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    char *tmp = twoExpHandler(exp1, exp2, '==');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t notEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    char *tmp = twoExpHandler(exp1, exp2, '!=');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t lesser(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    char *tmp = twoExpHandler(exp1, exp2, '<');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t lesserEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
   
    char *tmp = twoExpHandler(exp1, exp2, '<=');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};

}


expression_t greaterEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    char *tmp = twoExpHandler(exp1, exp2, '>=');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};

}


expression_t greater(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    char *tmp = twoExpHandler(exp1, exp2, '>');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t plus(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    char *tmp = twoExpHandler(exp1, exp2, '+');
    
    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t minus(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    char *tmp = twoExpHandler(exp1, exp2, '-');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t shiftLeft(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    char *tmp = twoExpHandler(exp1, exp2, '<<');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t shiftRight(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    char *tmp = twoExpHandler(exp1, exp2, '>>');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t multiply(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    char *tmp = twoExpHandler(exp1, exp2, '*');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t divide(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    char *tmp = twoExpHandler(exp1, exp2, '/');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t unaryMinus(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);

    char *tmp = oneExpHandler(exp, '--');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t unaryPlus(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);

    char *tmp = oneExpHandler(exp, '++');

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}

char * oneExpHandler(expression_t exp1, char operand){

    char *tmp1;

    //First expression to intermediate variable
    //Our logic always creates an intermediate
    if(exp1.exp_type == EXP_TYPE_LITERAL){
        sprintf(tmp1, "t%d", counter);
        fprintf(ir, "int t%d = %d\n", counter++, exp1.literal);
    }
    else if (exp1.exp_type == EXP_TYPE_VAR || exp1.exp_type == EXP_TYPE_TVALUE)
    {
        sprintf(tmp1, "t%d", counter);
        fprintf(ir, "int t%d = %s\n", counter++, exp1.var);
    }

    //temp char for output and return parameter
    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", counter++);

    fprintf(ir, "int %s %c %s;\n", tmp, operand, tmp1);

    return tmp;
}

char * twoExpHandler(expression_t exp1, expression_t exp2, char operand){
    //temps for later use -> literal or varname in tmp
    char tmp1[255];
    char tmp2[255];

    //First expression to intermediate variable
    //Our logic always creates an intermediate
    if(exp1.exp_type == EXP_TYPE_LITERAL){
        sprintf(tmp1, "t%d", counter);
        fprintf(ir, "int t%d = %d\n", counter++, exp1.literal);
    }
    else if (exp1.exp_type == EXP_TYPE_VAR || exp1.exp_type == EXP_TYPE_TVALUE)
    {
        sprintf(tmp1, "t%d", counter);
        fprintf(ir, "int t%d = %s\n", counter++, exp1.var);
    }

    //2nd expression
    if(exp2.exp_type == EXP_TYPE_LITERAL){
        sprintf(tmp2, "t%d", counter);
        fprintf(ir, "int t%d = %d\n", counter++, exp2.literal);
    }
    else if (exp2.exp_type == EXP_TYPE_VAR || exp2.exp_type == EXP_TYPE_TVALUE)
    {
        sprintf(tmp2, "t%d", counter);
        fprintf(ir, "int t%d = %s\n", counter++, exp2.var);
    }


    //temp char for output and return parameter
    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", counter++);

    fprintf(ir, "int %s = %s %c %s;\n", tmp, tmp1, operand, tmp2);

    return tmp;
}


funcCallParamList_t* addExprAsParam(int line, int col, funcCallParamList_t *paramList, expression_t exp)
{

    funcCallParamList_t* newParamList = (funcCallParamList_t*)malloc(sizeof(funcCallParamList_t));

     newParamList->exp = exp;
    
    if(paramList != NULL)
    {
        paramList->next = &newParamList->exp;

        newParamList->prev = &paramList->exp;
    }

    return newParamList;
}


expression_t evalArray(int line, int col, char *name, expression_t exp){


    //maybe has to be changed back, but not sure yet...
    //problem is: can not know if arr[0] = 2 or t0 = arr[0]
    
    if(checkForInt(line, col, exp));


    if(exp.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "int t%d = %s[%d];\n", counter, name, exp.literal);
    }
    else if(exp.exp_type == EXP_TYPE_VAR)
    {
        fprintf(ir, "int t%d = %s[%s];\n", counter, name, exp.var);
    }

    char tmp3[10];
    sprintf(tmp3, "t%d", counter++);


    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp3, NULL, NULL};

}


void ifStart(int line, int col, expression_t exp)
{
    if(checkForInt(line, col, exp));



    if(exp.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "IF(%d)GOTO \n", exp.literal);
    }
    else
    {
        fprintf(ir, "IF(%s)GOTO\n", exp.var);
    }
    



}


void ifEnd(int line, int col, int label)
{
    fprintf(ir, "l%d:\n", label);



}


void elseStart(int line, int col)
{



}


void elseEnd(int line, int col)
{




}
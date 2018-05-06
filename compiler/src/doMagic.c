#include "doMagic.h"


expression_t assign(int line, int col, expression_t exp1, expression_t exp2)
{

    checkIfAssignable(line, col, exp1);
    checkForInt(line, col, exp2);

    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%s = %d\n", exp1.var, exp2.literal);
    }
    else if (exp2.exp_type == EXP_TYPE_VAR)
    {
        fprintf(ir, "%s = %s\n", exp1.var, exp2.var);
    }

    
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t logicalOr(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t logicalAnd(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t logicalNot(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);

    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t equals(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t notEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t lesser(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t lesserEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t greaterEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t greater(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t plus(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t minus(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t shiftLeft(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t shiftRight(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t multiply(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t divide(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t unaryMinus(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
}


expression_t unaryPlus(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);
    return (expression_t) {EXP_TYPE_LITERAL, 1, NULL, NULL, NULL};
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




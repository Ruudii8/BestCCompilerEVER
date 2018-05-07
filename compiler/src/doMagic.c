#include "doMagic.h"

int counter = 0;

expression_t assign(int line, int col, expression_t exp1, expression_t exp2)
{
    checkIfAssignable(line, col, exp1);
    checkForInt(line, col, exp2);

    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "int %s = %d;\n", exp1.var, exp2.literal);
    }
    else if (exp2.exp_type == EXP_TYPE_VAR)
    {
        fprintf(ir, "int %s = %s;\n", exp1.var, exp2.var);
    }
    else if (exp2.exp_type == EXP_TYPE_TVALUE){
        fprintf(ir, "int %s = %s;\n", exp1.var, exp2.var);
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
    char tmp1[255];
    char tmp2[255];

    if(exp1.exp_type == EXP_TYPE_LITERAL){
        sprintf(tmp1, "%d", exp1.literal);
    }
    else if (exp1.exp_type == EXP_TYPE_VAR)
    {
        strcpy(tmp1, exp1.var);
    }
    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        sprintf(tmp2, "%d", exp2.literal);
    }
    else if (exp2.exp_type == EXP_TYPE_VAR)
    {
        strcpy(tmp2, exp2.var);
    }

    fprintf(ir, "int t%d = %s + %s;\n", counter, tmp1, tmp2);

    char tmp3[10];
    sprintf(tmp3, "t%d", counter++);


    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp3, NULL, NULL};
}


expression_t minus(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    char tmp1[255];
    char tmp2[255];

    if(exp1.exp_type == EXP_TYPE_LITERAL){
        sprintf(tmp1, "%d", exp1.literal);
    }
    else if (exp1.exp_type == EXP_TYPE_VAR)
    {
        strcpy(tmp1, exp1.var);
    }
    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        sprintf(tmp2, "%d", exp2.literal);
    }
    else if (exp2.exp_type == EXP_TYPE_VAR)
    {
        strcpy(tmp2, exp2.var);
    }

    fprintf(ir, "int t%d = %s + %s;\n", counter, tmp1, tmp2);

    char tmp3[10];
    sprintf(tmp3, "t%d", counter++);


    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp3, NULL, NULL};
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

expression_t evalArray(int line, int col, char *name, expression_t exp){
    
    if(checkForInt(line, col, exp))
    {
        
    }

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


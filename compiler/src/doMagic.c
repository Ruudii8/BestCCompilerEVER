#include "doMagic.h"

int counter = 0;

expression_t assign(int line, int col, expression_t exp1, expression_t exp2)
{
    checkIfAssignable(line, col, exp1);
    checkForInt(line, col, exp2);


    if(exp1.exp_type == EXP_TYPE_VAR || exp1.exp_type == EXP_TYPE_TVALUE)
    {
        fprintf(ir, "int %s = ", exp1.var);
    }
    else if(exp1.exp_type == EXP_TYPE_ARR)
    {
        fprintf(ir, "%s[%d] = ", exp1.var, exp1.literal);
    }


    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d;\n", exp2.literal);
    }
    else if (exp2.exp_type == EXP_TYPE_VAR || exp2.exp_type == EXP_TYPE_TVALUE)
    {
        fprintf(ir, "%s;\n", exp2.var);
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

    fprintf(ir, "int t%d = ", counter);

    if(exp1.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d", exp1.literal);
    }
    else
    {
        fprintf(ir, "%s", exp1.var);
    }

    fprintf(ir, " <= ");

    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d;\n", exp2.literal);
    }
    else
    {
        fprintf(ir, "%s;\n", exp2.var);
    }

    /*char tmp4[10];
    sprintf(tmp4, "t%d", counter++);
    

    char *tmp = tmp4;

    fprintf(ir, "   %s %s   ", tmp4, tmp);*/


    char tmp5[10];

    sprintf(tmp5, "t%d", counter);

    char *tmp = "test";

    fprintf(ir, "%s %p", tmp, &tmp);

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};

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

    fprintf(ir, "   %d   ", exp.exp_type);


    if(exp.exp_type == EXP_TYPE_TVALUE)
    {
        fprintf(ir, "tvalue var: %s %p\n", exp.var, &exp.var);
    }
    

    if(exp.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "IF(%d)GOTO\n", exp.literal);
    }
    else
    {
        fprintf(ir, "IF(%s)GOTO\n", exp.var);
    }
    



}


void ifEnd(int line, int col)
{




}


void elseStart(int line, int col)
{



}


void elseEnd(int line, int col)
{




}
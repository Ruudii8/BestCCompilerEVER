#include "doMagic.h"

int t_counter = 0;
int l_counter = 0;

expression_t assign(int line, int col, expression_t exp1, expression_t exp2)
{
    checkIfAssignable(line, col, exp1);
    checkForInt(line, col, exp2);

    //if type is literal print value, else print name -- Array needs to be handled different
    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "\tint %s = %d;\n", exp1.var, exp2.literal);
    }
    else
    {
        if (exp2.exp_type == EXP_TYPE_ARR)
        {
            exp2 = evalArray(line, col, exp2);
        }
        fprintf(ir, "\tint %s = %s;\n", exp1.var, exp2.var);
    }


    
    return exp2;
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

    if(exp1.exp_type == EXP_TYPE_ARR)
    {
        exp1 = evalArray(line, col, exp1);
    }

    if(exp2.exp_type == EXP_TYPE_ARR)
    {
        exp2 = evalArray(line, col, exp2);
    }


    fprintf(ir, "\tIF (%s <= %s) GOTO l%d\n", exp1.var, exp2.var, l_counter++);
    fprintf(ir, "\tint t%d = 0;\n", t_counter);
    fprintf(ir, "\tGOTO l%d\n", l_counter++);
    fprintf(ir, "l%d:\n", l_counter-2);
    fprintf(ir, "\tint t%d = 1;\n", t_counter);
    fprintf(ir, "l%d:\n", l_counter-1);

    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);


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
        sprintf(tmp1, "t%d", t_counter);
        fprintf(ir, "int t%d = %d\n", t_counter++, exp1.literal);
    }
    else if (exp1.exp_type == EXP_TYPE_VAR || exp1.exp_type == EXP_TYPE_TVALUE)
    {
        sprintf(tmp1, "t%d", t_counter);
        fprintf(ir, "int t%d = %s\n", t_counter++, exp1.var);
    }

    //temp char for output and return parameter
    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

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
        sprintf(tmp1, "t%d", t_counter);
        fprintf(ir, "\tint t%d = %d\n", t_counter++, exp1.literal);
    }
    else if (exp1.exp_type == EXP_TYPE_VAR || exp1.exp_type == EXP_TYPE_TVALUE)
    {
        sprintf(tmp1, "t%d", t_counter);
        fprintf(ir, "\tint t%d = %s\n", t_counter++, exp1.var);
    }

    //2nd expression
    if(exp2.exp_type == EXP_TYPE_LITERAL){
        sprintf(tmp2, "t%d", t_counter);
        fprintf(ir, "\tint t%d = %d\n", t_counter++, exp2.literal);
    }
    else if (exp2.exp_type == EXP_TYPE_VAR || exp2.exp_type == EXP_TYPE_TVALUE)
    {
        sprintf(tmp2, "t%d", t_counter);
        fprintf(ir, "\tint t%d = %s\n", t_counter++, exp2.var);
    }


    //temp char for output and return parameter
    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

    fprintf(ir, "\tint %s = %s %c %s;\n", tmp, tmp1, operand, tmp2);

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


expression_t evalArray(int line, int col, expression_t exp)
{

    if(checkForInt(line, col, exp));

    if(exp.exp->exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "\tint t%d = %s[%d];\n", t_counter, exp.var, exp.exp->literal);
    }
    else
    {
        fprintf(ir, "\tint t%d = %s[%s];\n", t_counter, exp.var, exp.exp->var);
    }

    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


int ifStart(int line, int col, expression_t exp)
{

    if(exp.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "\tIF (%d == 0) GOTO l%d\n",exp.literal, l_counter++);
    }
    else
    {
        fprintf(ir, "\tIF (%s == 0) GOTO l%d\n",exp.var, l_counter++);
    }


    return l_counter - 1;

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
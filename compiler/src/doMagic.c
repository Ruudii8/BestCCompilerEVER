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
        else if(exp2.exp_type == EXP_TYPE_FUNC)
        {
            exp2 = evalFunc(line, col, exp2);
        }
        fprintf(ir, "\tint %s = %s;\n", exp1.var, exp2.var);
    }
    
    return exp2;
}


expression_t logicalOr(int line, int col, expression_t exp1, expression_t exp2)
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

    fprintf(ir, "\tIF (");
    if(exp1.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d != 0", exp1.literal);
    }
    else
    {
        fprintf(ir, "%s != 0", exp1.var);
    }
    fprintf(ir, "\t) GOTO l%d;\n", l_counter);

    fprintf(ir, "\tIF (");
    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d != 0", exp2.literal);
    }
    else
    {
        fprintf(ir, "%s != 0", exp2.var);
    }
    fprintf(ir, "\t) GOTO l%d;\n", l_counter++);

    fprintf(ir, "\tint t%d = 0;\n", t_counter);
    fprintf(ir, "\tGOTO l%d;\n", l_counter++);
    fprintf(ir, "l%d:\n", l_counter-2);
    fprintf(ir, "\tint t%d = 1;\n", t_counter);
    fprintf(ir, "l%d:\n", l_counter-1);

    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t logicalAnd(int line, int col, expression_t exp1, expression_t exp2)
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

    fprintf(ir, "\tIF (");
    if(exp1.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d == 0", exp1.literal);
    }
    else
    {
        fprintf(ir, "%s == 0", exp1.var);
    }
    fprintf(ir, "\t) GOTO l%d;\n", l_counter);

    fprintf(ir, "\tIF (");
    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d == 0", exp2.literal);
    }
    else
    {
        fprintf(ir, "%s == 0", exp2.var);
    }
    fprintf(ir, "\t) GOTO l%d;\n", l_counter++);

    fprintf(ir, "\tint t%d = 1;\n", t_counter);
    fprintf(ir, "\tGOTO l%d;\n", l_counter++);
    fprintf(ir, "l%d:\n", l_counter-2);
    fprintf(ir, "\tint t%d = 0;\n", t_counter);
    fprintf(ir, "l%d:\n", l_counter-1);

    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t logicalNot(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);

    if(exp.exp_type == EXP_TYPE_ARR)
    {
        exp = evalArray(line, col, exp);
    }

    fprintf(ir, "\tIF (");
    if(exp.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d == 0", exp.literal);
    }
    else
    {
        fprintf(ir, "%s == 0", exp.var);
    }
    fprintf(ir, "\t) GOTO l%d\n", l_counter);

    fprintf(ir, "\tint t%d = 0;\n", t_counter);
    fprintf(ir, "\tGOTO l%d;\n", l_counter++);
    fprintf(ir, "l%d:\n", l_counter-2);
    fprintf(ir, "\tint t%d = 1;\n", t_counter);
    fprintf(ir, "l%d:\n", l_counter-1);

    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


expression_t equals(int line, int col, expression_t exp1, expression_t exp2)
{
    return (expression_t)relOp(line, col, exp1, exp2, "==");
}


expression_t notEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    return (expression_t)relOp(line, col, exp1, exp2, "!=");
}


expression_t lesser(int line, int col, expression_t exp1, expression_t exp2)
{
    return (expression_t)relOp(line, col, exp1, exp2, "<");
}


expression_t lesserEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    return (expression_t)relOp(line, col, exp1, exp2, "<=");
}


expression_t greaterEquals(int line, int col, expression_t exp1, expression_t exp2)
{
    return (expression_t)relOp(line, col, exp1, exp2, ">=");
}


expression_t greater(int line, int col, expression_t exp1, expression_t exp2)
{
    return (expression_t)relOp(line, col, exp1, exp2, ">");
}


expression_t plus(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);
    
    return (expression_t)arithmOp(line, col, exp1, exp2, "+");
}


expression_t minus(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    return (expression_t)arithmOp(line, col, exp1, exp2, "-");
}


expression_t shiftLeft(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    return (expression_t)arithmOp(line, col, exp1, exp2, "<<");
}


expression_t shiftRight(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    return (expression_t)arithmOp(line, col, exp1, exp2, ">>");
}


expression_t multiply(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    return (expression_t)arithmOp(line, col, exp1, exp2, "*");
}


expression_t divide(int line, int col, expression_t exp1, expression_t exp2)
{
    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);

    return (expression_t)arithmOp(line, col, exp1, exp2, "/");
}


expression_t unaryMinus(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);

    expression_t exp_tmp = (expression_t) {EXP_TYPE_LITERAL, 0, NULL, NULL, NULL};

    return (expression_t)arithmOp(line, col, exp_tmp, exp, "-");
}


expression_t unaryPlus(int line, int col, expression_t exp)
{
    checkForInt(line, col, exp);

    return exp;
}


expression_t arithmOp(int line, int col, expression_t exp1, expression_t exp2, char* operand)
{


    checkForInt(line, col, exp1);
    checkForInt(line, col, exp2);


    if (exp1.exp_type == EXP_TYPE_ARR)
    {
        exp1 = evalArray(line, col, exp1);
    }
    else if(exp1.exp_type == EXP_TYPE_FUNC)
    {
        exp1 = evalFunc(line, col, exp1);
    }

    if (exp2.exp_type == EXP_TYPE_ARR)
    {
        exp2 = evalArray(line, col, exp2);
    }
    else if(exp2.exp_type == EXP_TYPE_FUNC)
    {
        exp2 = evalFunc(line, col, exp2);
    }


    fprintf(ir, "\tint t%d = ", t_counter);

    if(exp1.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d %s ", exp1.literal, operand);
    }
    else
    {
        fprintf(ir, "%s %s ", exp1.var, operand);
    }

    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d;\n", exp2.literal);
    }
    else
    {
        fprintf(ir, "%s;\n", exp2.var);
    }

    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};

}


expression_t relOp(int line, int col, expression_t exp1, expression_t exp2, char* operand)
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

    if(exp1.exp_type == EXP_TYPE_FUNC)
    {
        exp1 = evalFunc(line, col, exp1);
    }

    if(exp2.exp_type == EXP_TYPE_FUNC)
    {
        exp2 = evalFunc(line, col, exp2);
    }

    fprintf(ir, "\tIF (");

    if(exp1.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d %s ", exp1.literal, operand);
    }
    else
    {
        fprintf(ir, "%s %s ", exp1.var, operand);
    }

    if(exp2.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "%d", exp2.literal);
    }
    else
    {
        fprintf(ir, "%s", exp2.var);
    }

    fprintf(ir, "\t) GOTO l%d;\n", l_counter++);
    fprintf(ir, "\tint t%d = 0;\n", t_counter);
    fprintf(ir, "\tGOTO l%d;\n", l_counter++);
    fprintf(ir, "l%d:\n", l_counter-2);
    fprintf(ir, "\tint t%d = 1;\n", t_counter);
    fprintf(ir, "l%d:\n", l_counter-1);

    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
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



void checkIfFuncCall(int line, int col, expression_t exp)
{
    if(exp.exp_type == EXP_TYPE_FUNC)
    {

        if(exp.paramList != NULL)
        {

            if(exp.paramList->exp.exp_type == EXP_TYPE_FUNC)
            {
                exp.paramList->exp = evalFunc(line, col, exp.paramList->exp);
            }

            while(exp.paramList->prev != NULL)
            {
                if(exp.paramList->exp.exp_type == EXP_TYPE_FUNC)
                {
                    exp.paramList->exp = evalFunc(line, col, exp.paramList->exp);
                }

                exp.paramList = exp.paramList->prev;
            }

            fprintf(ir, "\tCALL %s, (", exp.var);

            while(exp.paramList != NULL)
            {
                if(exp.paramList->exp.exp_type == EXP_TYPE_LITERAL)
                {
                    fprintf(ir, "%d,", exp.paramList->exp.literal);
                }
                else
                {
                    fprintf(ir, "%s,", exp.paramList->exp.var);
                }

                exp.paramList = exp.paramList->next;
            }

            fprintf(ir, ");\n");
        }
        else
        {
            fprintf(ir, "\tCALL %s;\n", exp.var);
        }
    }

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


expression_t evalFunc(int line, int col, expression_t exp)
{

    if(exp.paramList != NULL)
    {

        if(exp.paramList->exp.exp_type == EXP_TYPE_FUNC)
        {
            exp.paramList->exp = evalFunc(line, col, exp.paramList->exp);
        }

        while(exp.paramList->prev != NULL)
        {
            if(exp.paramList->exp.exp_type == EXP_TYPE_FUNC)
            {
                exp.paramList->exp = evalFunc(line, col, exp.paramList->exp);
            }

            exp.paramList = exp.paramList->prev;

        }

        fprintf(ir, "\tint t%d = CALL %s, (", t_counter, exp.var);

        while(exp.paramList != NULL)
        {
            if(exp.paramList->exp.exp_type == EXP_TYPE_LITERAL)
            {
                fprintf(ir, "%d,", exp.paramList->exp.literal);
            }
            else
            {
                fprintf(ir, "%s,", exp.paramList->exp.var);
            }

            exp.paramList = exp.paramList->next;
        }

        fprintf(ir, ");\n");
    }
    else
    {
        fprintf(ir, "\tint t%d = CALL %s;\n", t_counter, exp.var);
    }

    char *tmp = malloc( sizeof(char) * (3 + 1 ) );
    sprintf(tmp, "t%d", t_counter++);

    return (expression_t) {EXP_TYPE_TVALUE, NULL, tmp, NULL, NULL};
}


int ifStart(int line, int col, expression_t exp)
{
    if(checkForInt(line, col, exp));


    if(exp.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "\tIF (%d == 0) GOTO l%d;\n",exp.literal, l_counter++);
    }
    else
    {
        fprintf(ir, "\tIF (%s == 0) GOTO l%d;\n",exp.var, l_counter++);
    }


    return l_counter - 1;

}


void ifEnd(int line, int col, int label)
{
    fprintf(ir, "l%d:\n", label);
}


void elseEnd(int line, int col, int label)
{
    fprintf(ir, "l%d:\n", label);
}


void whileEnd(int line, int col, int jump, int label)
{
    fprintf(ir, "\tGOTO l%d;\n", jump);
    fprintf(ir, "l%d:\n", label);
}


int createJump()
{
    fprintf(ir, "\tGOTO l%d;\n", l_counter++);

    return l_counter-1;
}


void createMainJump()
{
    fprintf(ir, "\tGOTO main;\n");
}


int createLabel()
{
    fprintf(ir, "l%d:\n", l_counter++);
    return l_counter-1;
}


void createFunctionLabel(char *label)
{
    fprintf(ir, "%s:\n", label);
}


void returnVoid(int line, int col)
{
    checkReturnVoid(line, col);

    fprintf(ir, "\tRETURN;");
}


void returnInt(int line, int col, expression_t exp)
{
    checkReturnInt(line, col, exp);

    if(exp.exp_type == EXP_TYPE_LITERAL)
    {
        fprintf(ir, "\tRETURN %d;\n", exp.literal);
    }
    else
    {
        if(exp.exp_type == EXP_TYPE_ARR)
        {
            exp = evalArray(line, col, exp);
        }
        if(exp.exp_type == EXP_TYPE_FUNC)
        {
            exp = evalFunc(line, col, exp);
        }
        fprintf(ir, "\tRETURN %s;\n", exp.var);
    }
}
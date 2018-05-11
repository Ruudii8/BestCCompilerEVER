/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%code requires {	
	// Project-specific includes
  #include "uthash.h"
  #include "diag.h"
  #include "types.h"
  #include "symboltable.h"
  #include "typecheck.h"
  #include "logger.h"
  #include "doMagic.h"

  void yyerror (const char*);
  extern int yylex(void);

}

%union {
  int i;
  char *id;
  int type;
  var_tmp_t var_tmp;
  expression_t exp;
  funcCallParamList_t *paramList;
  int label;
}
 
// Verbose error messages
%error-verbose

%locations
%start program

// Compiler in debugging information
%debug

// Enable debug output
%initial-action
{
	yydebug = 0;
};

/*
 * One shift/reduce conflict is expected for the "dangling-else" problem. This
 * conflict however is solved by the default behavior of bison for shift/reduce 
 * conflicts (shift action). The default behavior of bison corresponds to what
 * we want bison to do: SHIFT if the lookahead is 'ELSE' in order to bind the 'ELSE' to
 * the last open if-clause. 
 */
%expect 1

%token DO WHILE
%token IF ELSE
%token INT VOID
%token RETURN

%token COLON COMMA SEMICOLON

%token BRACE_OPEN BRACE_CLOSE
%token BRACKET_OPEN BRACKET_CLOSE
%token PARA_OPEN PARA_CLOSE

%token <id>ID
%token <i> NUM

%token ASSIGN 
%token LOGICAL_OR
%token LOGICAL_AND
%token EQ NE     
%token LS LSEQ GTEQ GT 
%token SHIFT_LEFT SHIFT_RIGHT
%token PLUS MINUS     
%token MUL DIV MOD
%token LOGICAL_NOT UNARY_MINUS UNARY_PLUS

%type <type> type
%type <var_tmp> identifier_declaration
%type <type> variable_declaration
%type <exp> expression
%type <label> jump_expression
%type <exp> primary
%type <exp> function_call
%type <paramList> function_call_parameters
%type <label> label
%type <label> jump

%right ASSIGN
%left LOGICAL_OR
%left LOGICAL_AND
%left EQ NE
%left LS LSEQ GTEQ GT
%left SHIFT_LEFT SHIFT_RIGHT
%left PLUS MINUS
%left MUL DIV MOD
%right UNARY_PLUS UNARY_MINUS LOGICAL_NOT
%left PARA_OPEN PARA_CLOSE
%left BRACE_OPEN BRACE_CLOSE
%left BRACKET_OPEN BRACKET_CLOSE
%left COLON COMMA SEMICOLON

%%

program
     : {init(); createMainJump();} program_element_list {printAll(@2.last_line, @2.last_column);}
     ;

program_element_list
     : program_element_list program_element 
     | program_element 
     ;

program_element
     : variable_declaration SEMICOLON                       
     | function_declaration SEMICOLON
     | function_definition
     | SEMICOLON
     ;
									
type
     : INT {$$ = TYPE_INT;}
     | VOID {$$ = TYPE_VOID;}
     ;

variable_declaration
     : variable_declaration COMMA identifier_declaration {addVariable(@1.first_line, @1.first_column, $3, $1);}
     | type identifier_declaration {addVariable(@1.first_line, @1.first_column, $2, $1); $$ = $1;}
     ;

identifier_declaration
     : ID BRACKET_OPEN NUM BRACKET_CLOSE {$$ = (var_tmp_t){ $1, $3 };}
     | ID {$$ = (var_tmp_t){ $1, -1 };}
     ;

function_definition
     : type ID PARA_OPEN PARA_CLOSE {defineFunction(@1.first_line, @1.first_column, $2, $1, NULL); createFunctionLabel($2);} BRACE_OPEN stmt_list BRACE_CLOSE {endFunctionScope(@1.first_line, @1.first_column);}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE {defineFunction(@1.first_line, @1.first_column, $2, $1, returnParameters()); createFunctionLabel($2);} BRACE_OPEN stmt_list BRACE_CLOSE {endFunctionScope(@1.first_line, @1.first_column);}
     ;

function_declaration
     : type ID PARA_OPEN PARA_CLOSE {declareFunction(@1.first_line, @1.first_column, $2, $1, NULL);}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE {declareFunction(@1.first_line, @1.first_column, $2, $1, returnParameters());}
     ;

function_parameter_list
     : function_parameter 
     | function_parameter_list COMMA function_parameter
     ;
	
function_parameter
     : type identifier_declaration {addParameter(@1.first_line, @1.first_column, $2, $1);}
     ;
									
stmt_list
     : /* empty: epsilon */
     | stmt_list stmt
     ;

stmt
     : stmt_block
     | variable_declaration SEMICOLON
     | expression SEMICOLON
     | stmt_conditional
     | stmt_loop
     | RETURN expression SEMICOLON {returnInt(@1.first_line, @1.first_column, $2);}
     | RETURN SEMICOLON {returnVoid(@1.first_line, @2.first_column);}
     | SEMICOLON /* empty statement */
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
	
stmt_conditional
     : IF PARA_OPEN jump_expression PARA_CLOSE stmt {ifEnd(@1.first_line, @1.first_column, $3);}
     | IF PARA_OPEN jump_expression PARA_CLOSE stmt ELSE jump {ifEnd(@1.first_line, @1.first_column, $3);} stmt {elseEnd(@1.first_line, @1.first_column, $7);}
     ;
									
stmt_loop
     : WHILE label PARA_OPEN jump_expression PARA_CLOSE stmt {whileEnd(@1.first_line, @1.first_column, $2, $4);}
     | DO label stmt WHILE PARA_OPEN jump_expression PARA_CLOSE SEMICOLON {whileEnd(@1.first_line, @1.first_column, $2, $6);}
     ;

jump_expression
     : expression {$$ = ifStart(@1.first_line, @1.first_column, $1);}
     ;

label
     : %empty {$$ = createLabel();}
     ;

jump
     : %empty {$$ = createJump();}
									
expression
     : expression ASSIGN expression {$$ = assign(@1.first_line, @1.first_column, $1, $3);}
     | expression LOGICAL_OR expression {$$ = logicalOr(@1.first_line, @1.first_column, $1, $3);}
     | expression LOGICAL_AND expression {$$ = logicalAnd(@1.first_line, @1.first_column, $1, $3);}
     | LOGICAL_NOT expression {$$ = logicalNot(@1.first_line, @1.first_column, $2);}
     | expression EQ expression {$$ = equals(@1.first_line, @1.first_column, $1, $3);}
     | expression NE expression {$$ = notEquals(@1.first_line, @1.first_column, $1, $3);}
     | expression LS expression {$$ = lesser(@1.first_line, @1.first_column, $1, $3);}
     | expression LSEQ expression {$$ = lesserEquals(@1.first_line, @1.first_column, $1, $3);}
     | expression GTEQ expression {$$ = greaterEquals(@1.first_line, @1.first_column, $1, $3);}
     | expression GT expression {$$ = greater(@1.first_line, @1.first_column, $1, $3);}
     | expression PLUS expression {$$ = plus(@1.first_line, @1.first_column, $1, $3);}
     | expression MINUS expression {$$ = minus(@1.first_line, @1.first_column, $1, $3);}
     | expression SHIFT_LEFT expression {$$ = shiftLeft(@1.first_line, @1.first_column, $1, $3);}
     | expression SHIFT_RIGHT expression {$$ = shiftRight(@1.first_line, @1.first_column, $1, $3);}
     | expression MUL expression {$$ = multiply(@1.first_line, @1.first_column, $1, $3);}
     | expression DIV expression {$$ = divide(@1.first_line, @1.first_column, $1, $3);}
     | MINUS expression %prec UNARY_MINUS {$$ = unaryMinus(@1.first_line, @1.first_column, $2);}
     | PLUS expression %prec UNARY_PLUS {$$ = unaryPlus(@1.first_line, @1.first_column, $2);}
     | ID BRACKET_OPEN primary BRACKET_CLOSE {expression_t *e = malloc(sizeof(expression_t)); *e = $3;$$ =(expression_t){EXP_TYPE_ARR, NULL, $1, NULL, e};}
     | PARA_OPEN expression PARA_CLOSE {$$ = $2;}
     | function_call {checkFuncCallParams(@1.first_line, @1.first_column, $1); $$ = $1; }
     | primary {$$ = $1;}
     ;

primary
     : NUM {$$ = (expression_t){EXP_TYPE_LITERAL, $1, NULL, NULL, NULL};}                     
     | ID {$$ = (expression_t){EXP_TYPE_VAR, NULL, $1, NULL, NULL};}
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE {$$ = (expression_t){EXP_TYPE_FUNC, NULL, $1, NULL, NULL};}
      | ID PARA_OPEN function_call_parameters PARA_CLOSE {$$ = (expression_t){EXP_TYPE_FUNC, NULL, $1, $3, NULL};}
      ;

function_call_parameters
     : function_call_parameters COMMA expression {$$ = addExprAsParam(@1.first_line, @1.first_column, $1, $3);}
     | expression {$$ = addExprAsParam(@1.first_line, @1.first_column, NULL , $1);}
     ;

%%

void yyerror (const char *msg)
{
	FATAL_COMPILER_ERROR(INVALID_SYNTAX, 0, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}

/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%code requires {	
	// Project-specific includes
  #include "uthash.h"
  #include "diag.h"
  #include "symboltable.h"
  #include "typecheck.h"
  #include "types.h"

  void yyerror (const char*);
  extern int yylex(void);

}

%union {
  int i;
  char *id;
  int type;
  var_tmp_t var_tmp;
  expression_t exp;
  funcCallParamList_t paramList;
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
%type <exp> primary
%type <exp> function_call
%type <paramList> function_call_parameters

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
     : {init();} program_element_list {printAll(@2.last_line, @2.last_column);}
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
     : type ID PARA_OPEN PARA_CLOSE {defineFunction(@1.first_line, @1.first_column, $2, $1, NULL);} BRACE_OPEN stmt_list BRACE_CLOSE {endFunctionScope(@1.first_line, @1.first_column);}
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE {defineFunction(@1.first_line, @1.first_column, $2, $1, returnParameters());} BRACE_OPEN stmt_list BRACE_CLOSE {endFunctionScope(@1.first_line, @1.first_column);}
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
     | RETURN expression SEMICOLON
     | RETURN SEMICOLON
     | SEMICOLON /* empty statement */
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
	
stmt_conditional
     : IF PARA_OPEN expression PARA_CLOSE stmt
     | IF PARA_OPEN expression PARA_CLOSE stmt ELSE stmt
     ;
									
stmt_loop
     : WHILE PARA_OPEN expression PARA_CLOSE stmt
     | DO stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON
     ;
									
expression
     : expression ASSIGN expression {checkAssignment(@1.first_line, @1.first_column, &$1, &$3);}
     | expression LOGICAL_OR expression
     | expression LOGICAL_AND expression
     | LOGICAL_NOT expression
     | expression EQ expression
     | expression NE expression
     | expression LS expression 
     | expression LSEQ expression 
     | expression GTEQ expression 
     | expression GT expression
     | expression PLUS expression
     | expression MINUS expression
     | expression SHIFT_LEFT expression
     | expression SHIFT_RIGHT expression
     | expression MUL expression
     | expression DIV expression
     | MINUS expression %prec UNARY_MINUS
     | PLUS expression %prec UNARY_PLUS
     | ID BRACKET_OPEN primary BRACKET_CLOSE {$$ = (expression_t){EXP_TYPE_VAR, NULL, $1, NULL, &$3};}
     | PARA_OPEN expression PARA_CLOSE {$$ = $2;}
     | function_call {$$ = $1;}
     | primary {$$ = $1;}
     ;

primary
     : NUM {$$ = (expression_t){EXP_TYPE_LITERAL, $1, NULL, NULL, NULL};}                     
     | ID {$$ = (expression_t){EXP_TYPE_VAR, NULL, $1, NULL, NULL};}
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE {$$ = (expression_t){EXP_TYPE_FUNC, NULL, $1, NULL, NULL};}
      | ID PARA_OPEN function_call_parameters PARA_CLOSE {$$ = (expression_t){EXP_TYPE_FUNC, NULL, $1, NULL, NULL};}
      ;

function_call_parameters
     : function_call_parameters COMMA expression {;}
     | expression {;}
     ;

%%

void yyerror (const char *msg)
{
	FATAL_COMPILER_ERROR(INVALID_SYNTAX, 0, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}

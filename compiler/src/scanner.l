/* 
 * scanner.l - Scanner utility for the DHBW compiler
 */

%{
	#include "parser.y.h"

	int yycolumn = 1;
	int lastLine = 1;

	#define YY_USER_ACTION yylloc.first_line = yylineno; if(lastLine<yylineno) {yycolumn = 0; lastLine = yylineno;} yylloc.first_column = yycolumn; yycolumn += yyleng;

%}

/* 
 * Default setting for flex which indicates that yytext, which is one of the main connections
 * between the scanner and parser, is handled as pointer of char instead of an array of char. 
*/

%pointer	

/* save the line number of terminals */
%option yylineno

/* finish after scanning the provided file */
%option noyywrap		

%%

do {return DO;}
while {return WHILE;}
if {return IF;}
else {return ELSE;}
int {return INT;}
void {return VOID;}
return {return RETURN;}

; {return SEMICOLON;}
, {return COMMA;}
: {return COLON;}

\{ {return BRACE_OPEN;}
\} {return BRACE_CLOSE;}
\[ {return BRACKET_OPEN;}
\] {return BRACKET_CLOSE;}
\( {return PARA_OPEN;}
\) {return PARA_CLOSE;}

[a-zA-Z]+ 							{yylval.id = strdup(yytext); return ID;}
0|[1-9][0-9]* 						{yylval.i = atoi(strdup(yytext)); return NUM;}

= {return ASSIGN;}
\|\| {return LOGICAL_OR;}
&& {return LOGICAL_AND;}
== {return EQ;}
!= {return NE;}
\< {return LS;}
\<= {return LSEQ;}
\>= {return GTEQ;}
\> {return GT;}
\<< {return SHIFT_LEFT;}
\>> {return SHIFT_RIGHT;}
\+ {return PLUS;}
- {return MINUS;}
\* {return MUL;}
\/ {return DIV;}
% {return MOD;}
! {return LOGICAL_NOT;}

\n { }

%%

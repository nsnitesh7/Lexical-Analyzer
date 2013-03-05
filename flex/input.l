%{
#include <stdio.h>
#include"y.scantab.h"
%}
DIGIT [0-9]
IDENTIFIER [A-Z_a-z][a-z0-9]*
KEYWORDS <typedef|short|sizeof|int|float|double|bool|extern|char|signed|unsigned|for|while|do|return|struct|const|void|switch|break|case|continue|goto|long|static|union|volatile|default|enum>
%%
{DIGIT}+"."{DIGIT}* { printf("<%d,%f>\n",FLT_CONST,atof(yytext)); }
{DIGIT}+ { printf("<%d,%d>\n",INT_CONST,atoi(yytext)); }
{KEYWORDS} { printf("<%d,\"%s\">\n",KEYOWRDS,yytext); }
{IDENTIFIER} { printf("<%d,\"%s\">\n",IDENTIFIER,yytext); }
[;]	{ printf("<%d,%d>\n",SPEC_SYM,SEMICOLON); }
[(]	{ printf("<%d,%d>\n",SPEC_SYM,LFT_PARAN); }
[)]	{ printf("<%d,%d>\n",SPEC_SYM,RGT_PARAN); }
[[]	{ printf("<%d,%d>\n",SPEC_SYM,L_SQR_BKT ); }
[]]	{ printf("<%d,%d>\n",SPEC_SYM,R_SQR_BKT ); }
[-][>]	{ printf("<%d,%d>\n",SPEC_SYM,MINS_GRTR); }
[{]	{ printf("<%d,%d>\n",SPEC_SYM,LFT_BRACE); }
[}]	{ printf("<%d,%d>\n",SPEC_SYM,RGT_BRACE); }
[,]	{ printf("<%d,%d>\n",SPEC_SYM,COMMA); }
[.]	{ printf("<%d,%d>\n",SPEC_SYM,DOTONLY ); }
[?]	{ printf("<%d,%d>\n",SPEC_SYM,QUES_MARK); }
[#]	{ printf("<%d,%d>\n",SPEC_SYM,INCLUDE); }
["]	{ printf("<%d,%d>\n",SPEC_SYM,DOUBLE_QUOTE); }
['']	{ printf("<%d,%d>\n",SPEC_SYM,SINGLE_QUOTE); }
[+]	{ printf("<%d,%d>\n",ARITH_OPTR,PLUS); }
[\-]	{ printf("<%d,%d>\n",ARITH_OPTR,MINS); }
[*]	{ printf("<%d,%d>\n",ARITH_OPTR,MULT); }
[/]	{ printf("<%d,%d>\n",ARITH_OPTR,DIVD); }
[%]	{ printf("<%d,%d>\n",ARITH_OPTR,PERCENT); }
[+][+]	{ printf("<%d,%d>\n",ARITH_OPTR,INCREMENT); }
[-][-]	{ printf("<%d,%d>\n",ARITH_OPTR,DECREMENT); }
[=]	{ printf("<%d,%d>\n",ASSGN_OPTR,EQUAL); }
[+][=]	{ printf("<%d,%d>\n",ASSGN_OPTR,PLUS_EQUAL); }
[-][=]	{ printf("<%d,%d>\n",ASSGN_OPTR,MINUS_EQUAL); }
[*][=]	{ printf("<%d,%d>\n",ASSGN_OPTR,MULT_EQUAL); }
[/][=]	{ printf("<%d,%d>\n",ASSGN_OPTR,DIV_EQUAL); }
[<]	{ printf("<%d,%d>\n",RELN_OPTR,LESS); }
[>]	{ printf("<%d,%d>\n",RELN_OPTR,GREATER); }
[<][=]	{ printf("<%d,%d>\n",RELN_OPTR,LESS_EQUAL); }
[>][=]	{ printf("<%d,%d>\n",RELN_OPTR,GRT_EQUAL); }
[=][=]	{ printf("<%d,%d>\n",RELN_OPTR,EQUAL_EQUAL); }
[!][=]	{ printf("<%d,%d>\n",RELN_OPTR,NOT_EQUAL); }
[&][&]	{ printf("<%d,%d>\n",BOOL_OPTR,BOOL_AND); }
[|][|]	{ printf("<%d,%d>\n",BOOL_OPTR,BOOL_OR); }
[!]	{ printf("<%d,%d>\n",BOOL_OPTR,BOOL_NOT); }
[<][<]	{ printf("<%d,%d>\n",SHIFT_OPTR,SHIFT_LEFT); }
[>][>]	{ printf("<%d,%d>\n",SHIFT_OPTR,SHIFT_RGT); }
[|]	{ printf("<%d,%d>\n",BIT_OPTR,BIT_OR); }
[&]	{ printf("<%d,%d>\n",BIT_OPTR,BIT_AND); }
[\^]	{ printf("<%d,%d>\n",BIT_OPTR,BIT_XOR); }
[~]	{ printf("<%d,%d>\n",BIT_OPTR,BIT_NOT); }
.|\n     ;
%%

int main()
{
        yylex();
}

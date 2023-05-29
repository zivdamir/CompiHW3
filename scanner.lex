%{

/* Declarations section */
#include <stdio.h>
#include "hw3_output.hpp"
#include "tokens.hpp"
#include "parser.tab.hpp"

void add_name_att(char* name);
void add_strVal_att(char* stVal);
void add_intVal_att(int numVal);
void add_type_att(char* type);

%}

%option yylineno
%option noyywrap

whitespace                                      ([ \t\n\r])
comment                                         (\/\/[^\r\n]*[\r]?[\n]?)
string                                          (\"([^\n\r\"\\]|\\[rnt"\\])+\")
num                                             (0|[1-9][0-9]*)
id                                              ([a-zA-Z][a-zA-Z0-9]*)
md_binop                                         ([*/])
pm_binop                                        ([-+])
relop                                           ([<>]=|>|<)
eqop                                            ([=!]=)
%%
void                                                                                {add_type_att("void"); return VOID;}
int                                                                                 {add_type_att("int"); return INT;}
byte                                                                                {add_type_att("byte"); return BYTE;}
b                                                                                   {add_type_att("byte"); return B;}
bool                                                                                {add_type_att("bool"); return BOOL;}
override                                                                            {return OVERRIDE;}
and                                                                                 return AND;
or                                                                                  return OR;
not                                                                                 return NOT;
true                                                                                {add_type_att("bool"); return TRUE;}
false                                                                               {add_type_att("bool"); return FALSE;}
return                                                                              return RETURN;
if                                                                                  return IF;
else                                                                                return ELSE;
while                                                                               return WHILE;
break                                                                               return BREAK;
continue                                                                            return CONTINUE;
;                                                                                   return SC;
,                                                                                   return COMMA;
\(                                                                                  return LPAREN;
\)                                                                                  return RPAREN;
\{                                                                                  return LBRACE;
\}                                                                                  return RBRACE;
=                                                                                   return ASSIGN;
{relop}                                                                             return RE_RELOP;
{eqop}                                                                              return EQ_RELOP;
{md_binop}                                                                          return MD_BINOP;
{pm_binop}                                                                          return PM_BINOP;
{id}                                                                                {add_name_att(yytext);return ID;}                                                                      
{num}                                                                               {add_type_att("int");add_numVal_att(atoi(yytext));add_strVal_att(yytext); return NUM;}    
{string}                                                                            {add_type_att("string");add_strVal_att(yytext);return STRING;}
{whitespace}*                                                                       ;
{comment}                                                                           ;
.                                                                                   {output::errorLex(yylineno);exit(0);}
%%

void add_name_att(char* name)
{
    yylval.name = string(name);
}
void add_strVal_att(char* strVal)
{
    yylval.strVal = string(strVal);
}
void add_numVal_att(int numVal)
{
    yylval.numVal = numVal;
}
void add_type_att(char* type)
{
    yylval.type = string(type);
}
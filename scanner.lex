%{
    

/* Declarations section */
#include <stdio.h>
#include <string>
#include "hw3_output.hpp"
#include "tokens.hpp"
#include "parser.tab.hpp"
using namespace std;
void add_name_att(char* name);
void add_strVal_att(char* stVal);
void add_intVal_att(int intVal);
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
void                                                                                {add_type_att("VOID"); return VOID;}
int                                                                                 {add_type_att("INT"); return INT;}
byte                                                                                {add_type_att("BYTE"); return BYTE;}
b                                                                                   {add_type_att("BYTE"); return B;}
bool                                                                                {add_type_att("BOOL"); return BOOL;}
override                                                                            {return OVERRIDE;}
and                                                                                 return AND;
or                                                                                  return OR;
not                                                                                 return NOT;
true                                                                                {add_type_att("BOOL"); return TRUE;}
false                                                                               {add_type_att("BOOL"); return FALSE;}
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
{num}                                                                               {add_type_att("INT");add_intVal_att(atoi(yytext));add_strVal_att(yytext); return NUM;}    
{string}                                                                            {add_type_att("STRING");add_strVal_att(yytext); return STRING;}
{whitespace}*                                                                       ;
{comment}                                                                           ;
.                                                                                   {output::errorLex(yylineno);exit(0);}
%%

void add_name_att(char* name)
{
    if(name != nullptr)
    {
        yylval.name = (char*) malloc(sizeof(char)*(strlen(name)+1));
        strcpy(yylval.name, name);
    }
    else
        yylval.name = nullptr;
}
void add_strVal_att(char* strVal)
{
    if(strVal != nullptr)
    {
        yylval.strVal = (char*) malloc(sizeof(char)*(strlen(strVal)+1));
        strcpy( yylval.strVal, strVal);
    }
    else
        yylval.strVal = nullptr;
}
void add_intVal_att(int intVal)
{
    yylval.intVal = intVal;
}
void add_type_att(char* type)
{
    if(type != nullptr)
    {
        yylval.type = (char*) malloc(sizeof(char)*(strlen(type)+1));
        strcpy( yylval.type, type);
    }
    else
        yylval.type = nullptr;
}



    
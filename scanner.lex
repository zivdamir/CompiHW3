%{

/* Declarations section */
#include <stdio.h>
#include "hw3_output.hpp"
#include "tokens.hpp"
#include "parser.tab.hpp"


void allocate(char* name, char* type);


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
void                                                                                {add_type_att("void");return VOID;}
int                                                                                 return INT;
byte                                                                                return BYTE;
b                                                                                   return B;
bool                                                                                return BOOL;
override                                                                            return OVERRIDE;
and                                                                                 return AND;
or                                                                                  return OR;
not                                                                                 return NOT;
true                                                                                return TRUE;
false                                                                               return FALSE;
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
{id}                                                                                return ID;                                                                      
{num}                                                                               {return NUM;}    
{string}                                                                            return STRING;
{whitespace}*                                                                       ;
{comment}                                                                           ;
.                                                                                   {output::errorLex(yylineno);exit(0);}
%%

void add_name_att(char* name)
{
    if(name != nullptr)
    {
        yylval.name = (char*) malloc(sizeof(char)*(strlen(name)+1));
        strcpy( yylval.name, name);
    }
    else
    {
        yylval.name = nullptr;
    }

}
void add_type_att(char* type)
{
    if(type != nullptr)
    {
        yylval.type = (char*) malloc(sizeof(char)*(strlen(type)+1));
        strcpy( yylval.type, type);
    }
    else
    {
        yylval.type = nullptr;
    }
}
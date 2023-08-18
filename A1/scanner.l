%{
// This part goes directly into the generated parser

#include "scanner.hh"
#include "parser.tab.hh"

// YY_DECL is bound to the type of the scanner. The default is 
// #define YY_DECL int Scanner::yylex()
// which is generated anyway.

//If you want the type of the generated scanner to be different, do this

#undef  YY_DECL
#define YY_DECL int IPL::Scanner::yylex( IPL::Parser::semantic_type * const lval, IPL::Parser::location_type *location )

//We want these to be the first action after a token is recognized. 

#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

// token_type is a short hand for IPL::Parser::token

//using token_type = IPL::Parser::token;

#define token IPL::Parser::token

%}

/* %option debug */
%option nodefault
%option yyclass="Scanner"
%option noyywrap
%option c++

delim [ \t\n]
ws  {delim}+
digit [0-9]
INT_CONSTANT {digit}+
FLOAT_CONSTANT {digit}+(\.{digit}*)?([Ee][+-]?{digit}+)?|\.{digit}+([Ee][+-]?{digit}+)?
STRING_LITERAL \"([^\\\"]|\\.)*\"

%%        case token::EOFILE: return "EOFILE";


%{ /** Code executed at the beginning of yylex **/

      yylval = lval;

%}



[ \t\n]                                       //skip
"//".*
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]
int                    {
yylval->emplace< std::string >( YYText() );
                    return token::INT;
} 
void                  {
yylval->emplace< std::string >( YYText() );
                    return token::VOID;
} 
float         {
yylval->emplace< std::string >( YYText() );
return token::FLOAT;
} 
struct      {
yylval->emplace< std::string >( YYText() );
return token::STRUCT;
} 
return {
yylval->emplace< std::string >( YYText() );
 return token::RETURN;
} 
if {
yylval->emplace< std::string >( YYText() );
     return token::IF;
} 
for   {
yylval->emplace< std::string >( YYText() );
 return token::FOR;
} 
while {
yylval->emplace< std::string >( YYText() );
  return token::WHILE;
} 
else {
yylval->emplace< std::string >( YYText() );
  return token::ELSE;
}
[a-zA-Z_][a-zA-Z0-9_]*        {
yylval->emplace< std::string >( YYText() );
        return token::IDENTIFIER;
} 
"["                                         return '[';
"]"                                         return ']';
"{"                                         return '{';
"}"                                         return '}';
"+"                                         return '+';
","                                         return ',';
"("                                         return '(';
")"                                         return ')';
"!"                                         return '!';
"-"                                         return '-';
"&"                                         return '&';
";"                                         return ';';
"*"   return '*';
"/"   return '/';
"="   return '=';
">"   return '>';
"<"   return '<';
"."   return '.';
"++" {
yylval->emplace< std::string >( YYText() );
return token::INC_OP;
} 
"->" {
yylval->emplace< std::string >( YYText() );
return token::PTR_OP;
} 
"<=" {
yylval->emplace< std::string >( YYText() );
return token::LE_OP;
} 
">=" {
yylval->emplace< std::string >( YYText() );
return token::GE_OP;
} 
"!=" {
yylval->emplace< std::string >( YYText() );
return token::NE_OP;
} 
"==" {
yylval->emplace< std::string >( YYText() );
return token::EQ_OP;
} 
"&&" {
yylval->emplace< std::string >( YYText() );
return token::AND_OP;
} 
"||" {
  yylval->emplace< std::string >( YYText() );
return token::OR_OP;
} 
[0-9]*|0[bB][01]*|0[0-7]*|0[xX][0-9a-fA-F]* {
yylval->emplace< std::string >( YYText() );
return token::INT_CONSTANT;
} 
{FLOAT_CONSTANT} {
yylval->emplace< std::string >( YYText() );
return token::FLOAT_CONSTANT;
} 
{STRING_LITERAL} {
yylval->emplace< std::string >( YYText() );
return token::STRING_LITERAL;
} 
.				       {
yylval->emplace< std::string >( YYText() );
return token::OTHERS;
} 

<<EOF>>                                  yyterminate();
%%

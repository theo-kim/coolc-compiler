/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */

%option noyywrap

%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
        if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
                YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/* errors */
#define STRLEN_ERR      "String constant too long"
#define STR_NULL_ERR    "String contains null character"
#define STR_TERM_ERR    "Unterminated string constant"
#define STR_EOF_ERR     "EOF in string constant"
#define COM_EOF_ERR     "EOF in comment"
#define COM_UNMATCH_ERR "Unmatched *)"

/* prototypes */
bool update_str_buff (char c);
int throw_err (const char *msg);
int throw_line_err (const char *msg); /* No recovery on new line */
int throw_invalid_char (char *c);

/* Variables */

int comment_count = 0;
char str_buff_ref[MAX_STR_CONST];
char *str_buff = str_buff_ref;

%}

/*
 * Define names for regular expressions here.
 */

/* exclusive START conditions */
%x  COMMENT
%x  STRING
%x  ESCAPE_STRING
%x  ESCAPE_RESOLVE
%x  ERROR_STATE
%x  CLASS_START
%x  CLASS_FEATURE_START

COMMENT_OPEN_BLOCK  "(*"
COMMENT_CLOSE_BLOCK "*)"
COMMENT_INLINE      --.*\n

STRING_LIT          \"
INT_LIT             [0-9]+
BOOL_LIT            ([t](?i:rue))|([f](?i:alse))

/* Keywords */
CLASS           (?i:class)
ELSE            (?i:else)
FI              (?i:fi)
IF              (?i:if)
IN              (?i:in)
INHERITS        (?i:inherits)
LET             (?i:let)
LOOP            (?i:loop)
POOL            (?i:pool)
THEN            (?i:then)
WHILE           (?i:while)
CASE            (?i:case)
ESAC            (?i:esac)
OF              (?i:of)
NEW             (?i:new)
NOT             (?i:not)
ISVOID          (?i:isvoid)
DARROW          =>
ASSIGN          <-
LE              <=

/* Operators and Symbols */
ARITHMETIC_OP   [+-/*~]
BOOLEAN_OP      [=<]
BRACKETS        [{}()]

/* Identifiers */    
TYPE_ID         [A-Z][A-Za-z0-9_]*
OBJECT_ID       [a-z][A-Za-z0-9_]*

/* Whitespace */
WHITESPACE      [[:blank:]\n\f\r\t\v]

%%
<*>\n                                 { curr_lineno++; REJECT; }
<ERROR_STATE><<EOF>>                  { yyterminate(); }

 /*
  *  COMMENTS
  */

<INITIAL,COMMENT>{COMMENT_OPEN_BLOCK}   { ++comment_count;
                                          BEGIN(COMMENT); }
<INITIAL>{COMMENT_CLOSE_BLOCK}          { return throw_line_err(COM_UNMATCH_ERR); }
<COMMENT>{COMMENT_CLOSE_BLOCK}          { if (--comment_count == 0) BEGIN(INITIAL); }
<COMMENT>.|\n                           {}
<COMMENT><<EOF>>                        { return throw_err(COM_EOF_ERR); }
{COMMENT_INLINE}                        { curr_lineno += 1; }

 /*
  * Special Symbols
  */

{BRACKETS}                              { return yytext[0]; }
":"                                     { return (':');}
";"                                     { return (';');}
"."                                     { return ('.');}
"@"                                     { return ('@');}
{ARITHMETIC_OP}                         { return yytext[0]; }
{BOOLEAN_OP}                            { return yytext[0]; }

 /*
  *  The multiple-character operators.
  */
{DARROW}               { return (DARROW); }

 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */

{CLASS}                 { return (CLASS); }
{ELSE}                  { return (ELSE); }
{FI}                    { return (FI); }
{IF}                    { return (IF); }
{IN}                    { return (IN); }
{INHERITS}              { return (INHERITS); }
{LET}                   { return (LET); }
{LOOP}                  { return (LOOP); }
{POOL}                  { return (POOL); }
{THEN}                  { return (THEN); }
{WHILE}                 { return (WHILE); }
{CASE}                  { return (CASE); }
{ESAC}                  { return (ESAC); }
{OF}                    { return (OF); }
{NEW}                   { return (NEW); }
{ASSIGN}                { return (ASSIGN); }
{LE}                    { return (LE); }
{ISVOID}                { return (ISVOID); }
{NOT}			{ return (NOT); }


 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */

 /* 
  * Literals
  */
{STRING_LIT}                            { 
                                          BEGIN(STRING); 
                                          str_buff = str_buff_ref; 
                                        }
 /* backslash indicates escaped character */
<STRING>[\\]     				{ BEGIN(ESCAPE_STRING); }
 /* non special characters are added to the string */
<ESCAPE_STRING>\n                       { BEGIN(STRING); }
<ESCAPE_STRING>[^btnf0\n]            {
                                          if (!update_str_buff(yytext[0])) {
                                            return throw_err(STRLEN_ERR);
                                          }
                                          BEGIN(STRING);
                                        }
 /* special characters translate to their special character */
<ESCAPE_STRING>[btnf]                   {
                                          char c = 0;
                                          switch (yytext[0]) {
                                            case 'b':
                                              c = 8; break;
                                            case 't':
                                              c = 9; break;
                                            case 'n':
                                              c = 10; break;
                                            case 'f':
                                              c = 12; break;
                                          }
                                          if (!update_str_buff(c)) 
                                            return throw_err(STRLEN_ERR);
                                          BEGIN(STRING); 
                                        }
<ESCAPE_STRING>0                        { return throw_err(STR_NULL_ERR); }

 /* Any character except backslash or double quotes */
<STRING>[^\"\\\n]{1}                     { 
                                          if (!update_str_buff(yytext[0])) {
                                            return throw_err(STRLEN_ERR);
                                          }
                                        }
<STRING>{STRING_LIT}                    { BEGIN(INITIAL);
                                          *str_buff = '\0';
                                          yylval.symbol = inttable.add_string(str_buff_ref, str_buff - str_buff_ref);
                                          return STR_CONST; }
<STRING>\n                              { BEGIN(INITIAL);
                                          return throw_line_err(STR_TERM_ERR); }
<ERROR_STATE>[^\"]*[\"]                 { BEGIN(INITIAL); }

{INT_LIT}                               { yylval.symbol = inttable.add_string(yytext, yyleng);
                                          return INT_CONST; }
{BOOL_LIT}                              {
                                          if (yytext[0] == 't') {
                                            yylval.boolean = 1;
                                          }
                                          else {
                                            yylval.boolean = 0;
                                          }
                                          return BOOL_CONST;
                                        }

 /*
  * Identifiers
  */

{TYPE_ID}               { yylval.symbol = inttable.add_string(yytext, yyleng);
                          return (TYPEID); }

{OBJECT_ID}             { yylval.symbol = inttable.add_string(yytext, yyleng); 
                          return (OBJECTID); }

 /*
  * Uncaptured whitespace
  */

{WHITESPACE}            {}

<*>.                    { return throw_invalid_char(yytext); }

%%

bool update_str_buff (char c) {
  if ((str_buff - str_buff_ref) + 2 > MAX_STR_CONST) {
    return false;
  }
  *str_buff++ = c;
  return true;
}

int throw_err (const char *msg) {
  yylval.error_msg = msg;
  BEGIN(ERROR_STATE);
  return ERROR;
}

int throw_invalid_char (char *c) {
  yylval.error_msg = c;
  return ERROR;
}

int throw_line_err (const char *msg) {
  yylval.error_msg = msg;
  return ERROR;
}
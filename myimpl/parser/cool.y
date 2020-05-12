/*
 *  cool.y
 *              Parser definition for the COOL language.
 *
 */
%{
#include <iostream>
#include "cool-tree.h"
#include "stringtab.h"
#include "utilities.h"

/* memory */
#define YYINITDEPTH 10000
#define YYMAXDEPTH 10000

extern char *curr_filename;

void yyerror(const char *s);        /*  defined below; called for each parse error */
extern int yylex();           /*  the entry point to the lexer  */

#define YYLTYPE int              /* the type of locations */
#define cool_yylloc curr_lineno
Symbol selfClass;
extern int node_lineno;          /* set before constructing a tree node
                                    to whatever you want the line number
                                    for the tree node to be */

/* The default action for locations.  Use the location of the first
   terminal/non-terminal and set the node_lineno to that value. */
#define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current = Rhs[1];                             \
  node_lineno = Current;

#define SET_NODELOC(Current)  \
  node_lineno = Current;


/************************************************************************/
/*                DONT CHANGE ANYTHING IN THIS SECTION                  */

Program ast_root;             /* the result of the parse  */
Classes parse_results;        /* for use in semantic analysis */
int omerrs = 0;               /* number of errors in lexing and parsing */
%}

/* A union of all the types that can be the result of parsing actions. Don't change this.*/
%union {
  Boolean boolean;
  Symbol symbol;
  Program program;
  Class_ class_;
  Classes classes;
  Feature feature;
  Features features;
  Formal formal;
  Formals formals;
  Case case_;
  Cases cases;
  Expression expression;
  Expressions expressions;
  char *error_msg;
}

/* 
   Declare the terminals; a few have types for associated lexemes.
   The token ERROR is never used in the parser; thus, it is a parse
   error when the lexer returns it.

   The integer following token declaration is the numeric constant used
   to represent that token internally.  Typically, Bison generates these
   on its own, but we give explicit numbers to prevent version parity
   problems (bison 1.25 and earlier start at 258, later versions -- at
   257)
*/
%token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
%token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
%token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
%token <symbol>  STR_CONST 275 INT_CONST 276 
%token <boolean> BOOL_CONST 277
%token <symbol>  TYPEID 278 OBJECTID 279 
%token ASSIGN 280 NOT 281 LE 282 ERROR 283

/*  DON'T CHANGE ANYTHING ABOVE THIS LINE, OR YOUR PARSER WONT WORK       */
/**************************************************************************/
 
   /* Complete the nonterminal list below, giving a type for the semantic
      value of each non terminal. (See section 3.6 in the bison 
      documentation for details). */

/* Declare types for the grammar's non-terminals. */
%type <program> program
%type <classes> class_list
%type <class_> class
%type <features> feature_list features
%type <feature> feature
%type <expression> expression let_list optional_assignment
%type <expressions> expression_list block_expression_list
%type <formals> formals
%type <formal> formal
%type <case_> case
%type <cases> cases

/* Precedence declarations go here. */

%right ASSIGN
%right NOT
%nonassoc LE '<' '='
%left '+' '-'
%left '*' '/'
%nonassoc ISVOID
%right '~'
%left '@'
%left '.'
%left '('

%%
/* 
   Save the root of the abstract syntax tree in a global variable.
*/
program : class_list    { @$ = @1; ast_root = program($1); }
        ;

class_list
        : class                 /* single class */
                { $$ = single_Classes($1);
                  parse_results = $$; }
        | class_list class      /* several classes */
                { $$ = append_Classes($1,single_Classes($2)); 
                  parse_results = $$; }
        
        ;

/* If no parent is specified, the class inherits from the Object class. */
class   : CLASS TYPEID '{' feature_list '}' ';'
                { $$ = class_($2,idtable.add_string("Object"),$4,
                              stringtable.add_string(curr_filename)); }
        | CLASS TYPEID INHERITS TYPEID '{' feature_list '}' ';'
                { $$ = class_($2,$4,$6,stringtable.add_string(curr_filename)); }
        /* Error handling */
        | CLASS error '{' feature_list '}' ';' 
                { yyclearin; }
        ;

/* Feature list may be empty, but no empty features in list. */
feature_list
        : features
                { $$ = $1; }
        | %empty  { $$ = nil_Features(); }
        ;

features
        : feature ';'
                { $$ = single_Features($1); }
        | features feature ';'
                { $$ = append_Features($1, single_Features($2)); }
        | error ';'
                { yyclearin; }
        ;

/* features can be either a method or an attribute */
/* features cannot be empty */
/* attributes may have an optional assignment expression */
/* The special value void is used as the default initialization for variables */
feature : OBJECTID ':' TYPEID optional_assignment
                { $$ = attr($1, $3, $4); }
        | OBJECTID '(' formals ')' ':' TYPEID '{' expression '}'
                { $$ = method($1, $3, $6, $8); }
        | OBJECTID '(' ')' ':' TYPEID '{' expression '}'
                { $$ = method($1, nil_Formals(), $5, $7); }
        /* Error Handling */
        ;

formals : formals ',' formal
                { $$ = append_Formals($1, single_Formals($3)); }
        | formal
                { $$ = single_Formals($1); }
        | error
                { yyclearin; }
        ;

formal  : OBJECTID ':' TYPEID
                { $$ = formal($1, $3); }
        ;

expression_list
        : %empty
                { $$ = nil_Expressions(); }
        | expression_list ',' expression
                { $$ = append_Expressions($1, single_Expressions($3)); }
        | expression
                { $$ = single_Expressions($1); }
        | error { yyclearin; }
        ;

block_expression_list
        : block_expression_list expression ';'
                { $$ = append_Expressions($1, single_Expressions($2)); }
        | expression ';'
                { $$ = single_Expressions($1); }
        | error ';' { yyclearin; }
        ;

expression
        /* constants */
        : STR_CONST 
                { $$ = string_const($1); }
        | INT_CONST 
                { $$ = int_const($1); }
        | BOOL_CONST
                { $$ = bool_const($1); }
        /* identifiers */
        | OBJECTID
                { $$ = object($1); }
        /* assignment */
        | OBJECTID ASSIGN expression
                { $$ = assign($1, $3); }
        /* dispatch */
        | expression '.' OBJECTID '(' expression_list ')'
                { $$ = dispatch($1, $3, $5); }
        | OBJECTID '(' expression_list ')'
                { $$ = dispatch(object(idtable.add_string("self")), $1, $3); }
        | expression '@' TYPEID '.' OBJECTID '(' expression_list ')'
                { $$ = static_dispatch($1, $3, $5, $7); }
        /* conditionals */
        | IF expression THEN expression ELSE expression FI
                { $$ = cond($2, $4, $6); }
        /* loop */
        | WHILE expression LOOP expression POOL
                { $$ = loop($2, $4); }
        /* block */
        | '{' block_expression_list '}'
                { $$ = block($2); }
        /* new */ 
        | NEW TYPEID
                { $$ = new_($2); }
        /* isvoid */
        | ISVOID expression
                { $$ = isvoid($2); }
        /* operators */
        | expression '+' expression
                { $$ = plus($1, $3); }
        | expression '-' expression
                { $$ = sub($1, $3); }
        | expression '/' expression
                { $$ = divide($1, $3); }
        | expression '*' expression
                { $$ = mul($1, $3); }
        | expression '<' expression
                { $$ = lt($1, $3); }
        | expression LE expression
                { $$ = leq($1, $3); }
        | expression '=' expression
                { $$ = eq($1, $3); }
        | '~' expression
                { $$ = neg($2); }
        | NOT expression
                { $$ = comp($2); }
        /* let */
        | LET let_list
                { $$ = $2; }
        /* case */
        | CASE expression OF cases ESAC
                { $$ = typcase($2, $4); }
        /* parenthesis */
        | '(' expression ')'
                { $$ = $2; }
        ;

optional_assignment
        : ASSIGN expression
                { $$ = $2; }
        | %empty
                { $$ = no_expr(); }

/* let statements are a treated as nesting */
/* the initialization portion is optional */
let_list
        : OBJECTID ':' TYPEID IN expression %prec ASSIGN
                { $$ = let($1, $3, no_expr(), $5); }
        | OBJECTID ':' TYPEID ASSIGN expression IN expression %prec ASSIGN
                { $$ = let($1, $3, $5, $7); }
        | OBJECTID ':' TYPEID ',' let_list
                { $$ = let($1, $3, no_expr(), $5); }
        | OBJECTID ':' TYPEID ASSIGN expression ',' let_list 
                { $$ = let($1, $3, $5, $7); }
        | error IN expression %prec ASSIGN
                { yyclearin; $$ = NULL; }
        | error ',' let_list 
                { yyclearin; $$ = NULL; }
        ;

case    : OBJECTID ':' TYPEID DARROW expression
                { $$ = branch($1, $3, $5); }
        ;

cases   : cases case ';'
                { $$ = append_Cases($1, single_Cases($2)); }
        | case ';'
                { $$ = single_Cases($1); }
        | error
                { }
        ;


/* end of grammar */
%%

/* This function is called automatically when Bison detects a parse error. Don't change this. */
void yyerror(const char *s)
{
  extern int curr_lineno;

  cerr << "\"" << curr_filename << "\", line " << curr_lineno << ": " \
    << s << " at or near ";
  print_cool_token(yychar);
  cerr << endl;
  omerrs++;

  if(omerrs>50) {fprintf(stdout, "More than 50 errors\n"); exit(1);}
}


%{
#include <stdio.h>
#include "alfa.h"
#include "generacion.h"
#include "tablaHash.h"
#include "tablaSimbolos.h"
#include "helpers.h"
#include <string.h>

#define LOCAL 1
#define GLOBAL 0

extern FILE* outF;
extern int yycolumn;
extern int yycolumnO;
extern int yylineno;
extern int yylex(void);
extern int tipoError;
int yyerror(char *s);
TABLA_HASH *tablaGlobal = NULL, *tablaLocal = NULL;
int tipo_actual;
int clase_actual;
int tamanio_vector_actual;
int pos_variable_local_actual = 1;
int pos_parametro_actual;
int num_variables_locales_actual;
int num_parametros_actual = 1;
int num_parametros_llamada_actual = 0;
int tipo_funcion_actual;
int fn_return = 0;
int ambito;
char spf[100];
INFO_SIMBOLO * busq;
int ncomp = 0;
int etiqueta = 0;
int en_explist = 0;
%}

%union {
	type_scanner scanner;
}

%token TOK_MONTH 
%token TOK_DAY
%token TOK_TIMESTAMP_1

%token TOK_ERROR

%type <scanner> tok_month
%type <scanner> tok_day
%type <scanner> tok_timestamp1
%type <scanner> date_1
%type <scanner> TOK_MONTH
%type <scanner> TOK_DAY  
%type <scanner> TOK_TIMESTAMP_1


%%

programa: date_1  { yycolumnO = yycolumn;};


date_1: tok_month tok_day tok_timestamp1 { 
	yycolumnO = yycolumn; 
	strcpy($$.text, $1.text); 
	$$.text[3]=' ';
	strcpy($$.text+4, $2.text);
	$$.text[6]=' ';
	strcpy($$.text+7, $3.text);
	printf( "%s, ", $$.text);
}

tok_month: TOK_MONTH { yycolumnO = yycolumn; strcpy($$.text, $1.text);}

tok_day: TOK_DAY { yycolumnO = yycolumn; strcpy($$.text, $1.text);}

tok_timestamp1: TOK_TIMESTAMP_1 { yycolumnO = yycolumn; strcpy($$.text, $1.text);}

fin: {
	
}

%%

int yyerror(char* s) {
	fprintf( stderr, "****Error sintactico en [lin %d, col %d]\n", yylineno, yycolumnO);
	return 1;
}

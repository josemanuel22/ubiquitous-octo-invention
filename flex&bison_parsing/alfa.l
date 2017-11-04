
%{
 #include <stdio.h>
 #include "alfa.h"
 #include "y.tab.h"
 #include <string.h>

 int yycolumn = 1;
 int yycolumnO = 1;
 int tipoError = 0;
 extern FILE *outF;
%}

%option noyywrap
%option yylineno

%%

[ ] {yycolumnO = ++yycolumn;}
[\t] {yycolumnO = yycolumn; yycolumn+=yyleng;}
\n { yycolumnO = yycolumn = 1; }

("Jan"|"Oct"|"Feb") {yycolumn += yyleng; strcpy(yylval.scanner.text, yytext); return TOK_MONTH; }
([0-2][0-9]|3[0-1]) {  yycolumn += yyleng; strcpy(yylval.scanner.text, yytext); return TOK_DAY; }
([0-1][1-9]|2[0-4]):([0-5][0-9]):([0-5][0-9]) {  yycolumn += yyleng; strcpy(yylval.scanner.text, yytext); return TOK_TIMESTAMP_1; }


. { fprintf( stderr, "****Error morfologico en [lin %d, col %d]: simbolo no permitido (%s)\n", yylineno, yycolumn, yytext); tipoError = 2; return TOK_ERROR; }

%%

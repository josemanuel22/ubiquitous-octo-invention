#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "alfa.h"
#include "y.tab.h"

extern FILE* yyin;
FILE* outF;

int main(int argc, char** argv) {

	clock_t start, end;
    double cpu_time_used;
    

    start = clock();
	if( argc < 3 ) exit( EXIT_FAILURE );
	
	yyin = fopen( argv[1], "r" );
	if( yyin == NULL ) {
		perror( "yyin no abierto\n" );
		exit( EXIT_FAILURE );
	}
	outF = fopen( argv[2], "w" );
	if( outF == NULL ) {
		fclose( yyin );
		perror( "outF no abierto\n" );
		exit( EXIT_FAILURE );
	}
    
    while (yyparse())

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("\nTIEMPO:%f\n", cpu_time_used);

	exit(EXIT_SUCCESS);
}

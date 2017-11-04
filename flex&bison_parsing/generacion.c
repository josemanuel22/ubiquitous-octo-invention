#include "generacion.h"


/**********************************************************************************/

/* FUNCIÓN PARA PODER HACER EL CÓDIGO MULTIPLATAFORMA U OTROS PARÁMETROS GENERALES TAL VEZ SE PUEDA QUEDAR VACÍA */
void escribir_cabecera_compatibilidad(FILE* fpasm)
{
}
/**********************************************************************************/

/* FUNCIÓN PARA ESCRIBIR LA SECCIÓN .data:
    MENSAJES GENERALES (TEXTOS)
    VARIABLES AUXILIARES NECESARIAS EN EL COMPILADOR QUE DEBAN TENER UN VALOR CONCRETO */
void escribir_subseccion_data(FILE* fpasm)
{
	fprintf(fpasm, "segment .data\n");
	fprintf( fpasm, "\t_msgdiv0 db \"****Error de ejecucion: Division por cero.\",0\n" );
	fprintf( fpasm, "\t_msgoutr db \"****Error de ejecucion: Indice fuera de rango.\",0\n" );

}
/**********************************************************************************/
/* FUNCIÓN PARA ESCRIBIR EL INICIO DE LA SECCIÓN .bss:
    AL MENOS HAY QUE DECLARAR LA VARIABLE AUXILIAR PARA GUARDAR EL PUNTERO DE PILA __esp
*/
void escribir_cabecera_bss(FILE* fpasm)
{
	fprintf(fpasm, "segment .bss\n");
	fprintf(fpasm, "\t__esp resd 1\n");
}

/**********************************************************************************/
/* GENERA EL CÓDIGO ASOCIADO EN LA SECCIÓN .bss PARA DECLARAR UNA VARIABLE CON
    SU NOMBRE (HAY QUE PRECEDER DE _)
    EL TAMANO (1 PARA VARIABLES QUE NO SEAN VECTORES O SU TAMANO EN OTRO CASO
    TIPO NOSOTROS USAREMOS ESTE AÑO ENTERO O BOOLEANO
*/
void declarar_variable(FILE* fpasm, char * nombre,  int tipo,  int tamano)
{
/* tipo no hace falta porque para nosotros todo es entero en esta versión, se deja por compatibilidad con futuras versiones*/
	fprintf(fpasm, "\t_%s resd %d\n", nombre, tamano);
}

/************************************************************************************/

/* ESCRIBE EL INICIO DE LA SECCIÓN DE CÓDIGO
    DECLARACIONES DE FUNCIONES QUE SE TOMARAN DE OTROS MODULOS
    DECLARACION DE main COMO ETIQUETA VISIBLE DESDE EL EXTERIOR
*/
void escribir_segmento_codigo(FILE* fpasm)
{
	fprintf(fpasm, "segment .text\n");
	fprintf(fpasm, "\tglobal main\n");
	fprintf(fpasm, "\textern scan_int, print_int, scan_float, print_float, scan_boolean, print_boolean\n");
	fprintf(fpasm, "\textern print_endofline, print_blank, print_string\n");
	fprintf(fpasm, "\textern alfa_malloc, alfa_free, ld_float\n");


}

/**********************************************************************************/

/* ESCRIBE EL PRINCIPIO DEL CÓDIGO PROPIAMENTE DICHO
    ETIQUETA DE INICIO
    SALVAGUARDA DEL PUNTERO DE PILA (esp) EN LA VARIABLE A TAL EFECTO (__esp)

*/
void escribir_inicio_main(FILE* fpasm)
{
	fprintf(fpasm, "main:\n");
	fprintf(fpasm, "\tmov dword [__esp], esp\n");
}

/**********************************************************************************/

/* ESCRITURA DEL FINAL DEL PROGRAMA
    GESTIÓN DE ERROR EN TIEMPO DE EJECUCIÓN (DIVISION POR 0)
    RESTAURACION DEL PUNTERO DE PILA A PARTIR DE LA VARIABLE __esp
    SENTENCIA DE RETORNO DEL PROGRAMA
*/
void escribir_fin(FILE* fpasm)
{
	fprintf( fpasm, "\tjmp near fin\n" );
	fprintf( fpasm, "\terror1:\n" );
	fprintf( fpasm, "\tpush dword _msgdiv0\n" );
	fprintf( fpasm, "\tcall print_string\n");
	fprintf( fpasm, "\tadd esp, 4\n");
	fprintf( fpasm, "\tcall print_endofline\n");
	fprintf( fpasm, "\tjmp near fin\n" );
	fprintf( fpasm, "\terror2:\n" );
	fprintf( fpasm, "\tpush dword _msgoutr\n" );
	fprintf( fpasm, "\tcall print_string\n");
	fprintf( fpasm, "\tadd esp, 4\n");
	fprintf( fpasm, "\tcall print_endofline\n");
	fprintf( fpasm, "\tfin:\n" );
	fprintf(fpasm, "\tmov dword esp, [__esp]\n");
	fprintf(fpasm, "\tret\n");
}

/**********************************************************************************/

/* SE INTRODUCE EL OPERANDO nombre EN LA PILA
    SI ES UNA VARIABLE (es_var == 1) HAY QUE PRECEDER EL NOMBRE DE _
    EN OTRO CASO, SE ESCRIBE TAL CUAL

*/
void escribir_operando(FILE * fpasm, char * nombre, int es_var)
{
	if( es_var ) fprintf(fpasm, "\tpush dword _%s\n", nombre);
	else fprintf(fpasm, "\tpush dword %s\n", nombre);

}
/**********************************************************************************/
void asignarVector(FILE * fpasm, int es_inmediato)
{
	fprintf(fpasm, "\tpop dword eax\n");
	if( !es_inmediato ) fprintf(fpasm, "\tmov dword eax, [eax]\n");
	fprintf(fpasm, "\tpop dword ebx\n");
	fprintf(fpasm, "\tmov dword [ebx], eax\n");

}

/**********************************************************************************/
/* ESCRIBE EL CÓDIGO PARA REALIZAR UNA ASIGNACIÓN DE LO QUE ESTÉ EN LA CIMA DE LA PILA A LA VARIABLE nombre
    SE RECUPERA DE LA PILA LO QUE HAYA POR EJEMPLO EN EL REGISTRO eax
    SI es_inmediato == 1 (ES UN VALOR) DIRECTAMENTE SE ASIGNA A LA VARIABLE _nombre  
    EN OTRO CASO es_inmediato == 0 (ES UNA DIRECCIÓN, UN NOMBRE DE VARIABLE) HAY QUE OBTENER SU VALOR DESREFERENCIANDO
EL VALOR ES [eax] */
void asignar(FILE * fpasm, char * nombre, int es_inmediato)
{
	fprintf(fpasm, "\tpop dword eax\n");
	if( !es_inmediato ) fprintf(fpasm, "\tmov dword eax, [eax]\n");
	fprintf(fpasm, "\tmov dword [_%s], eax\n", nombre);

}

/**********************************************************************************/
/* GENERA EL CÓDIGO PARA SUMAR LO QUE HAYA EN LAS DOS PRIMERAS (DESDE LA CIMA)
POSICIONES DE LA PILA, TENIENDO EN CUENTA QUE HAY QUE INDICAR SI SON OPERANDOS
INMEDIATOS O NO (VER ASIGNAR) ¡¡¡CUIDADO CON EL ORDEN !!!

*/
void sumar(FILE * fpasm, int es_inmediato_1, int es_inmediato_2)
{
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );

	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf( fpasm, "\tadd eax, ebx\n" );
	fprintf( fpasm, "\tpush dword eax\n" );
}
/**********************************************************************************/
/* GENERA EL CÓDIGO PARA CAMBIAR DE SIGNO LO QUE HAYA EN LA CIMA DE LA PILA
TENIENDO EN CUENTA QUE PUEDE SER UN VALOR INMEDIATO O UNA DIRECCION (VER ASIGNAR)

*/
void cambiar_signo(FILE * fpasm, int es_inmediato)
{

	fprintf(fpasm, "\tpop eax\n");
	if ( !es_inmediato ) fprintf(fpasm, "\tmov eax, [eax]\n");

	fprintf(fpasm, "\tneg eax\n");
	fprintf(fpasm, "\tpush dword eax\n");
}
/**********************************************************************************/
/* GENERA EL CÓDIGO PARA NEGAR COMO VALOR LÓGICO LO QUE HAYA EN LA CIMA DE LA PILA
TENIENDO EN CUENTA QUE PUEDE SER UN VALOR INMEDIATO O UNA DIRECCION (VER ASIGNAR)
COMO ES NECESARIO UTILIZAR ETIQUETAS, SE SUPONE QUE LA VARIABLE cuantos_no ES UN
CONTADOR QUE ASEGURA QUE UTILIZARLO COMO AÑADIDO AL NOMBRE DE LAS ETIQUETAS QUE
USEMOS (POR EJEMPLO cierto: O falso: ) NOS ASEGURARÁ QUE CADA LLAMADA A no
UTILIZA UN JUEGO DE ETIQUETAS ÚNICO

*/
void no(FILE * fpasm, int es_inmediato, int cuantos_no)
{

	fprintf(fpasm,"\tpop dword eax\n");
	if ( !es_inmediato ) fprintf(fpasm, "\tmov eax, [eax]\n");
	fprintf(fpasm,"\txor ebx, ebx\n");
	fprintf(fpasm,"\tcmp eax , ebx\n");
	fprintf(fpasm,"\tjz near _poner1_%d\n",cuantos_no);
	fprintf(fpasm,"\tpush dword ebx\n");
	fprintf(fpasm,"\tjmp near _nofin_%d\n",cuantos_no);
	fprintf(fpasm,"\t_poner1_%d:\n",cuantos_no);
	fprintf(fpasm,"\tmov eax,1\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\t_nofin_%d:\n",cuantos_no);


}

/**********************************************************************************/
/* GENERA EL CÓDIGO PARA LEER UNA VARIABLE DE NOMBRE nombre Y TIPO tipo (ESTE
AÑO SÓLO USAREMOS ENTERO Y BOOLEANO) DE CONSOLA LLAMANDO A LAS CORRESPONDIENTES
FUNCIONES DE ALFALIB (scan_int Y scan_boolean) */
void leer(FILE * fpasm, char * nombre, int tipo)
{
	fprintf(fpasm, "\tpush dword _%s\n", nombre);
	fprintf(fpasm, "\tcall scan_%s\n", tipo==ENTERO?"int":"boolean");
	fprintf(fpasm, "\tadd esp, 4\n");

}

/**********************************************************************************/
/* GENERA EL CÓDIGO PARA ESCRIBIR POR PANTALLA LO QUE HAYA EN LA CIMA DE LA PILA
TENIENDO EN CUENTA QUE PUEDE SER UN VALOR INMEDIATO (es_inmediato == 1) O UNA
DIRECCION (es_inmediato == 0) Y QUE HAY QUE LLAMAR A LA CORRESPONDIENTE
FUNCIÓN DE ALFALIB (print_int O print_boolean) DEPENDIENTO DEL TIPO (tipo == BOOLEANO
O ENTERO )
*/
void escribir(FILE * fpasm, int es_inmediato, int tipo)
{
	if( !es_inmediato ) fprintf(fpasm, "\tpop dword eax\n\tpush dword [eax]\n");
	fprintf(fpasm, "\tcall print_%s\n", tipo==ENTERO?"int":"boolean");
	fprintf(fpasm, "\tadd esp, 4\n");
	fprintf(fpasm, "\tcall print_endofline\n");

}

/**********************************************************************************/
void restar(FILE * fpasm, int es_inmediato_1, int es_inmediato_2)
{
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf( fpasm, "\tsub eax, ebx\n" );
	fprintf( fpasm, "\tpush dword eax\n" );
}
/**********************************************************************************/
void multiplicar(FILE * fpasm, int es_inmediato_1, int es_inmediato_2)
{
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf( fpasm, "\timul ebx\n" );
	fprintf( fpasm, "\tpush dword eax\n" );

}
/**********************************************************************************/
void dividir(FILE * fpasm, int es_inmediato_1, int es_inmediato_2)
{
	fprintf( fpasm, "\tpop dword ecx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ecx, [ecx]\n" );
	
	fprintf( fpasm, "\tcmp ecx, 0\n" );
	fprintf( fpasm, "\tjz near error1\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf( fpasm, "\tcdq\n" );

	fprintf( fpasm, "\tidiv ecx\n" );
	fprintf( fpasm, "\tpush dword eax\n" );

}
/**********************************************************************************/
void o(FILE * fpasm, int es_inmediato_1, int es_inmediato_2)
{
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf( fpasm, "\tor eax, ebx\n" );
	fprintf( fpasm, "\tpush dword eax\n" );
}

/**********************************************************************************/
void y(FILE * fpasm, int es_inmediato_1, int es_inmediato_2)
{
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf( fpasm, "\tand eax, ebx\n" );
	fprintf( fpasm, "\tpush dword eax\n" );
}

void menorigual( FILE * fpasm, int es_inmediato_1, int es_inmediato_2, int cuantos_mi ) {
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf(fpasm,"\tcmp eax , ebx\n");
	fprintf(fpasm,"\tjle near _poner1_%d\n",cuantos_mi);
	fprintf(fpasm,"\tpush dword ebx\n");
	fprintf(fpasm,"\tjmp near _poner0_%d\n",cuantos_mi);
	fprintf(fpasm,"\t_poner1_%d:\n",cuantos_mi);
	fprintf(fpasm,"\tmov eax,1\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\tjmp near _fin_%d\n",cuantos_mi);
	fprintf(fpasm,"\t_poner0_%d:\n",cuantos_mi);
	fprintf(fpasm,"\tmov eax,0\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\t_fin_%d:\n",cuantos_mi);	
}

void menor( FILE * fpasm, int es_inmediato_1, int es_inmediato_2, int cuantos_m ) {
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf(fpasm,"\tcmp eax , ebx\n");
	fprintf(fpasm,"\tjl near _poner1_%d\n",cuantos_m);
	fprintf(fpasm,"\tpush dword ebx\n");
	fprintf(fpasm,"\tjmp near _poner0_%d\n",cuantos_m);
	fprintf(fpasm,"\t_poner1_%d:\n",cuantos_m);
	fprintf(fpasm,"\tmov eax,1\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\tjmp near _fin_%d\n",cuantos_m);
	fprintf(fpasm,"\t_poner0_%d:\n",cuantos_m);
	fprintf(fpasm,"\tmov eax,0\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\t_fin_%d:\n",cuantos_m);
}

void mayorigual( FILE * fpasm, int es_inmediato_1, int es_inmediato_2, int cuantos_mi ) {
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf(fpasm,"\tcmp eax , ebx\n");
	fprintf(fpasm,"\tjge near _poner1_%d\n",cuantos_mi);
	fprintf(fpasm,"\tpush dword ebx\n");
	fprintf(fpasm,"\tjmp near _poner0_%d\n",cuantos_mi);
	fprintf(fpasm,"\t_poner1_%d:\n",cuantos_mi);
	fprintf(fpasm,"\tmov eax,1\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\tjmp near _fin_%d\n",cuantos_mi);
	fprintf(fpasm,"\t_poner0_%d:\n",cuantos_mi);
	fprintf(fpasm,"\tmov eax,0\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\t_fin_%d:\n",cuantos_mi);
}

void mayor( FILE * fpasm, int es_inmediato_1, int es_inmediato_2, int cuantos_m ) {
	fprintf( fpasm, "\tpop dword ebx\n" );
	if( !es_inmediato_2 ) fprintf( fpasm, "\tmov ebx, [ebx]\n" );
	
	fprintf( fpasm, "\tpop dword eax\n" );
	if( !es_inmediato_1 ) fprintf( fpasm, "\tmov eax, [eax]\n" );

	fprintf(fpasm,"\tcmp eax , ebx\n");
	fprintf(fpasm,"\tjg near _poner1_%d\n",cuantos_m);
	fprintf(fpasm,"\tpush dword ebx\n");
	fprintf(fpasm,"\tjmp near _poner0_%d\n",cuantos_m);
	fprintf(fpasm,"\t_poner1_%d:\n",cuantos_m);
	fprintf(fpasm,"\tmov eax,1\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\tjmp near _fin_%d\n",cuantos_m);
	fprintf(fpasm,"\t_poner0_%d:\n",cuantos_m);
	fprintf(fpasm,"\tmov eax,0\n");
	fprintf(fpasm,"\tpush dword eax\n");
	fprintf(fpasm,"\t_fin_%d:\n",cuantos_m);
}

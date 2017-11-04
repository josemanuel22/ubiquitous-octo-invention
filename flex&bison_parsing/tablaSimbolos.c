#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tablaHash.h"
#include "tablaSimbolos.h"



STATUS declararGlobal(TABLA_HASH *tabla_global, char* id, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2) {

	if(NULL == buscar_simbolo(tabla_global, id)){
		insertar_simbolo(tabla_global, id, categ, tipo, clase, adic1, adic2);
		return OK;
	} else {
		return ERR;
	}
}


STATUS declararLocal(TABLA_HASH *tabla_local, char* id, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2) {

	if(NULL == buscar_simbolo(tabla_local, id)){
		insertar_simbolo(tabla_local, id, categ, tipo, clase, adic1, adic2);
		return OK;
	} else {
		return ERR;
	}
}

INFO_SIMBOLO* usoGlobal(TABLA_HASH *tabla_global, char* id) {
	INFO_SIMBOLO* dato;
	dato = buscar_simbolo(tabla_global, id);
	return dato;
}

INFO_SIMBOLO* usoLocal(TABLA_HASH *tabla_global, TABLA_HASH *tabla_local, char* id) {
	INFO_SIMBOLO* dato;
	dato = buscar_simbolo(tabla_local, id);
	if(dato == NULL) {
		dato = buscar_simbolo(tabla_global, id);
	}
	return dato;
}

STATUS declararFuncion(TABLA_HASH *tabla_global, TABLA_HASH **tabla_local, char* id, TIPO tipo, CLASE clase, int adic1, int adic2) {
	int tam = 1000;

	if( NULL != buscar_simbolo(tabla_global, id)) {
		return ERR;
	} else {
		insertar_simbolo(tabla_global, id, FUNCION, tipo, clase, adic1, adic2);
		*tabla_local = crear_tabla(tam);
		insertar_simbolo(*tabla_local, id, FUNCION, tipo, clase, adic1, adic2);
	}
	return OK;
}

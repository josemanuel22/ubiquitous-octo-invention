#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H


STATUS declararGlobal(TABLA_HASH *tabla_global, char* id, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2);

STATUS declararLocal(TABLA_HASH *tabla_local, char* id, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2);

INFO_SIMBOLO* usoGlobal(TABLA_HASH *tabla_global, char* id);

INFO_SIMBOLO* usoLocal(TABLA_HASH *tabla_global, TABLA_HASH *tabla_local, char* id);

STATUS declararFuncion(TABLA_HASH *tabla_global, TABLA_HASH **tabla_local, char* id, TIPO tipo, CLASE clase, int adic1, int adic2);

#endif

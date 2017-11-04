FLAGS = -g 

.PHONY: all

all: alfal alfay
	gcc $(FLAGS) -o pruebaComp alfa.c lex.yy.c y.tab.c generacion.c tablaHash.c tablaSimbolos.c helpers.c

alfal: alfa.l
	flex alfa.l

alfay: alfa.y
	bison -dyv alfa.y

clean:
	rm pruebaComp lex.yy.c y.tab.c y.tab.h y.output

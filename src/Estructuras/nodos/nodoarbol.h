#ifndef NODO_ARBOL_H
#define NODO_ARBOL_H
#include <stdlib.h>

typedef struct nodoA
{
	struct nodoA *izq;
	struct nodoA *dch;
	void *dato;
}NodoA;

NodoA* crearNodoA(void *dato);

#endif

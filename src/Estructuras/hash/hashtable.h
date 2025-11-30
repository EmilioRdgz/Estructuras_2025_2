#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> //ELIPSIS
#include "../nodos/nodo.h"
#include "../cola/cola.h"

typedef struct
{
	void **tabla;
	int tam;
	int cant;
	int (*folding)(void*);
	void (*imprimir)(void*);
	int (*comparar)(void*,void*);
}HashTable;

int hash(int clave,int tam);
int rehash(int indice,int k,int tam);
HashTable inicializarHashTable(int tam,void (*imprimir)(void*),int (*comparar)(void*,void*));
int insertarClave(HashTable *hashtable,void *dato);
Cola buscarClave(HashTable *hashtable,void *dato);
void imprimirHashTabla(HashTable *hashtable);





#endif

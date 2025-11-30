#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include "../nodos/nodo.h"

typedef struct
{
	Nodo *inicio;
	int (*comparar)(void*,void*);	
	void (*imprimir)(void*);
	int cant;		
}Lista;

void mostrarLista(Lista);
void insertarFinal(Lista*,void*);
void borrarLista(Lista *);
void borrarDato(Lista *lista,void* dato,int  (*)(void*,void*));
void* buscarDato(Lista lista,void *dato,int  (*)(void*,void*));

void reordenar(Lista *,int  (*)(void*,void*) );

void insertarOrdenado(Lista *lista,void* dato);
void insertarInicio(Lista *lista,void* dato);
void insertarEnPosicion(Lista *lista,void* dato,int pos);
void eliminarEnPosicion(Lista *lista,int pos);

#endif


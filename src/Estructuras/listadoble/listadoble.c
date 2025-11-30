#include "listadoble.h"
#include "../../practica2/Alumno.h"
#include <stdio.h>


ListaD inicializarListaD(void)
{
	return (ListaD) {NULL,NULL,0,NULL,NULL,NULL};
}
void mostrarListaD(ListaD lista){
	int opcion;
	NodoD *p;
	//printf("\n Lista[%d]: ",lista.cant);

  do{
    printf("\n1. Imprimir Normal 2. Imprimir Inverso 3. Regresar\n");
    scanf("%d",&opcion);
    switch(opcion){
    case 1:
      printf("\nLista[%d]: ",lista.cant);
      for(p = lista.inicio; p!=NULL; p = p->sig){
        printf(" ");
        lista.imprimir(p->dato);
        printf("->>");
      }
      break;
      //printf("NULL");
      case 2:
      	printf("\nLista[%d]: ",lista.cant);
        for(p = lista.fin; p != NULL; p = p->ant){
          printf(" ");
          lista.imprimir(p->dato);
          printf("->");
        }
        break;
        //printf("NULL");
    }
  }while(opcion!=3);
}

void* buscarDato(ListaD lista,void *dato)
{
	NodoD *actual,*anterior=NULL;
	for( actual = lista.inicio ; actual!=NULL ;  actual = actual->sig)
	{
		if( lista.comparar(dato,actual->dato) == 0)
		{
			return actual->dato;
		}
		anterior = actual;
	}
	return NULL;

}

void insertarOrdenado(ListaD *lista, void *dato) {
	NodoD *nuevo = crearNodoD(dato);
	NodoD *actual, *anterior = NULL;

	for(actual = lista->inicio; actual != NULL; actual = actual->sig) {
		if(lista->comparar(dato, actual->dato) <= 0) {
			if (anterior != NULL) {
				// Insertar en medio
				nuevo->sig = actual;
				nuevo->ant = anterior;
				anterior->sig = nuevo;
				actual->ant = nuevo;
			} else {
				// Insertar en inicio
				nuevo->sig = actual;
				actual->ant = nuevo;
				lista->inicio = nuevo;
			}
			lista->cant++;
			return; // Salimos, ya insertamos
		}
		anterior = actual;
	}


	if(lista->inicio == NULL) {
		// Lista vacía
		lista->inicio = nuevo;
		lista->fin = nuevo;
	} else {
		// Insertar al final
		anterior->sig = nuevo;
		nuevo->ant = anterior;
		lista->fin = nuevo;
	}
	lista->cant++;
}




void reordenarLista(ListaD *lista){

	int opcion;
	printf("RReordenar por: 1. Matrícula, 2. Nombre, 3. Semestres, 4. Promedio\n");
	scanf("%d", &opcion);

	switch(opcion){
		case 1: lista->comparar = &compararPorMatricula;
		break;
		case 2: lista->comparar = &compararPorNombre;
		break;
		case 3: lista->comparar = &compararPorSemestres;
		break;
		case 4: lista->comparar = &compararPorPromedio;
		break;
		default: printf("Opcion incorrecto.\n"); return;
	}


	ListaD aux;
	aux.inicio = NULL;
    aux.fin = aux.inicio;
	aux.cant = 0;
	aux.comparar = lista->comparar;
	aux.imprimir = lista->imprimir;

	NodoD *p = lista->inicio;
	while(p != NULL){

		insertarOrdenado(&aux, p->dato);
		p = p->sig;
	}
	borrarLista(lista);
	*lista = aux;
}

void borrarLista(ListaD *lista)
{
	NodoD *actual,*anterior = NULL;
	for( actual = lista->inicio ; actual!=NULL ;  )
	{
		anterior = actual;
		actual = actual->sig;
		//free(anterior->dato);
		free(anterior);
	}
	lista->inicio = NULL;
    lista->fin = NULL;
	lista->cant = 0;
}

void borrarDato(ListaD *lista, void* dato)
{
	NodoD *actual = lista->inicio;

	while(actual != NULL)
	{
		if(lista->comparar(dato, actual->dato) == 0)
		{
			// Nodo del medio o final
			if(actual->ant != NULL)
				actual->ant->sig = actual->sig;
			else
				lista->inicio = actual->sig; // Nodo al inicio

			if(actual->sig != NULL) //si tenemos dato enfrente (borrar en medio)
				actual->sig->ant = actual->ant;
			else
				lista->fin = actual->ant; // Nodo al final

			free(actual);
			lista->cant--;
			return;
		}
		actual = actual->sig;
	}
}

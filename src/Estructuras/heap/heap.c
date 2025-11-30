#include "heap.h"


Heap inicializarHeap(void (*imprimir)(void*),int (*comparar)(void*,void*) )
{
	Arbol arbol = (Arbol){NULL,0,imprimir,NULL,NULL};
	return (Heap){NULL,0,arbol,imprimir,comparar,MIN};
}

void insertarHeap(Heap *heap, void *dato)
{
	NodoA **aux;
	heap->cantidad++;
	aux = NULL;
	while(!aux)
		aux = realloc(heap->arr,sizeof(NodoA*)*heap->cantidad);
	heap->arr = aux;
	heap->arr[heap->cantidad-1] = crearNodoA(dato);
	heapify(heap);
}

void* quitarHeap(Heap *heap)
{
	if(heap->cantidad == 0)
		return NULL;
	heap->cantidad--;
	void *dato = heap->arr[0]->dato;
	free(heap->arr[0]);
	heap->arr[0] = heap->arr[heap->cantidad];
	//DECREMENTAR MEMORIA	
	heap->arr = realloc(heap->arr,sizeof(NodoA*)*heap->cantidad);
	heapify(heap);	
	return dato;
}

/*
void heapMin(Heap *heap,int p)
{
	if( p < 0)
		return;
	int minimo = p;
	int hi = HIJO_IZQUIERDO(p);
	int hd = HIJO_DERECHO(p);
	if( (hi<heap->cantidad) && heap->comparar(heap->arr[hi]->dato,heap->arr[minimo]->dato)<0)
		minimo = hi;
	if( (hd<heap->cantidad) && heap->comparar(heap->arr[hd]->dato,heap->arr[minimo]->dato)<0)
		minimo = hd;
	if( minimo!= p)
	{
		NodoA *aux = heap->arr[p];
		heap->arr[p] = heap->arr[minimo];
		heap->arr[minimo] = aux;
		heapMin(heap,minimo);
	}
	else
		heapMin(heap,p-1);	
}
void heapMax(Heap *heap,int p)
{
	if( p < 0)
		return;
	int maximo = p;
	int hi = HIJO_IZQUIERDO(p);
	int hd = HIJO_DERECHO(p);
	if( (hi<heap->cantidad) && heap->comparar(heap->arr[hi]->dato,heap->arr[maximo]->dato)>0)
		maximo = hi;
	if( (hd<heap->cantidad) && heap->comparar(heap->arr[hd]->dato,heap->arr[maximo]->dato)>0)
		maximo = hd;
	if( maximo!= p)
	{
		NodoA *aux = heap->arr[p];
		heap->arr[p] = heap->arr[maximo];
		heap->arr[maximo] = aux;
		heapMax(heap,maximo);
	}
	else
		heapMax(heap,p-1);	
	
}
void heapify(Heap *heap)
{
	int p = heap->cantidad/2-1;
	switch(heap->tipo)
	{
		case MIN:
			heapMin(heap,p);
			break;
		case MAX:
			heapMax(heap,p);
			break;
	}
}
*/

void actualizarArbol(Heap *heap)
{
	int i,hi,hd;;
	for( i = 0 ; i < heap->cantidad ; i++)
	{		
		hi = HIJO_IZQUIERDO(i);
		hd = HIJO_DERECHO(i);
		heap->arr[i]->izq = (hi<heap->cantidad) ? heap->arr[hi]:NULL;
		heap->arr[i]->dch = (hd<heap->cantidad) ? heap->arr[hd]:NULL;
	}
	if(heap->arr)
	{
		heap->arbol.raiz = heap->arr[0];
		heap->arbol.cantidad = heap->cantidad;
	}
	else 
	{
		heap->arbol.raiz = NULL;
		heap->arbol.cantidad = 0;
	}
}

int siCambiar(Heap *heap,int indice,int hijo)
{
	switch(heap->tipo)
	{
		case MAX:		
			if(heap->comparar(heap->arr[hijo]->dato,heap->arr[indice]->dato)>0)
				return 1;
			break;
		case MIN:		
			if(heap->comparar(heap->arr[hijo]->dato,heap->arr[indice]->dato)<0)
				return 1;
			break;
	}
	return 0;
}	
void heapTipo(Heap *heap,int p)
{
	if( p < 0)
		return;
	int indice = p;
	int hi = HIJO_IZQUIERDO(p);
	int hd = HIJO_DERECHO(p);
	if( (hi<heap->cantidad) && siCambiar(heap,indice,hi))
		indice = hi;
	if( (hd<heap->cantidad) && siCambiar(heap,indice,hd))
		indice = hd;
	if( indice!= p)
	{
		NodoA *aux = heap->arr[p];
		heap->arr[p] = heap->arr[indice];
		heap->arr[indice] = aux;
		heapTipo(heap,indice);
	}
	else
		heapTipo(heap,p-1);	
}
void heapify(Heap *heap)
{
	int p = heap->cantidad/2-1;
	heapTipo(heap,p);
	actualizarArbol(heap);
}


void imprimirHeap(Heap heap)
{
	int i;
	for( i = 0; i< heap.cantidad ; i++)
	{
		heap.imprimir( heap.arr[i]->dato);
	}
    printf("\n");
}

void cambiarPrioridad(Heap *heap){
	// si heap es de tipo minimo entonces cambiar a tipo Max y si no cambiar a MIN (es decir estab en Max) y hacer heapify
  heap->tipo = (heap->tipo == MIN) ? MAX:MIN;
  heapify(heap);
}

void eliminarHeapPos(Heap *heap, int pos){
  if(!heap || heap->cantidad == 0 || pos < 0 || pos >= heap->cantidad)
    return;

  int ultimo = heap->cantidad - 1;
  NodoA *nodoElim = heap->arr[pos];

  //Si no es el utlimo entonces mover el ultimo nodo a la posicion a eliminar
  if(pos != ultimo){
    heap->arr[pos] = heap->arr[ultimo];
  }

  //reducir cant y memoria
  heap->cantidad--;
  if(heap->cantidad > 0){
    heap->arr = realloc(heap->arr,sizeof(NodoA*)*heap->cantidad);
  }else{
    free(heap->arr);
    heap->arr = NULL;
  }

  heapify(heap);

  free(nodoElim->dato);
  free(nodoElim);

}

void vaciarHeap(Heap *heap){
  if(!heap || heap->cantidad == 0){
    printf("No hay archivos a eliminar\n");
    return;
  }

  for(int i = 0; i < heap->cantidad; i++){
    free(heap->arr[i]->dato);
    free(heap->arr[i]);
  }

  free(heap->arr);
  heap->arr = NULL;
  heap->cantidad = 0;

  heap->arbol.raiz = NULL;
  heap->arbol.cantidad = 0;
}


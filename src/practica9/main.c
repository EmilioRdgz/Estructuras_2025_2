#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../captura/captura.h"
#include "../Estructuras/heap/heap.h"

typedef struct{
  char nombre[20];
  int numPaginas;
  //apuntador a función para imprimir archivo
  //apuntador a función para comparar archivo (se compara por el numero de paginas que tiene)
}Archivo;

int compararEntero(void*,void*);
void imprimirEntero(void*);
Archivo *crearArchivo(char *nombre, int numPaginas);
int compararArchivo(void *a, void *b);
void imprimirArchivo(void *a);

int main(void)
{
  Heap heap = inicializarHeap( &imprimirArchivo,&compararArchivo);
  heap.arbol.imprimir = heap.imprimir;
  int opcion;
  do{
    //rintf("\nHeap de los 5 primeros archivos\n");
  	printf("\n\n[0] CAMBIAR PRIORIDAD\n");
  	printf("[1] MOSTRAR COLA DE IMPRESIÓN\n");
  	printf("[2] AGREGAR ARCHIVO\n");
  	printf("[3] PROCESAR/IMPRIMIR ARCHIVO\n");
    printf("[4] ELIMINAR ARCHIVO\n");
    printf("[5] ELIMINAR TODOS LOS ARCHIVOS\n");
    printf("[6] TERMINAR PROGRAMA\n");
  	scanf("%d", &opcion);
  	clear_buffer();

    switch (opcion){
      case 0:
        printf("\nHeap Antes: \n");
    	imprimirHeap(heap);
    	printf("\n");
    	imprimirArbol(heap.arbol);
        printf("\n");

        cambiarPrioridad(&heap);
        heapify(&heap);
        printf("Cambiado de tipo\n");

        printf("\nHeap Después: \n");
        imprimirHeap(heap);
        printf("\n");
        imprimirArbol(heap.arbol);
        printf("\n\n");
        break;
      case 1:
        imprimirHeap(heap);
        printf("\n");
        imprimirArbol(heap.arbol);
        printf("\n\n");
        break;
      case 2:{
        printf("\nHeap Antes: \n");
    	imprimirHeap(heap);
    	printf("\n");
    	imprimirArbol(heap.arbol);
        printf("\n");

        char nombre[30];
        int numPaginas;
        inputCadena("\nNombre del archivo: ",nombre, 30);
        inputEntero("\nNumero de paginas: ",&numPaginas);
        Archivo *nuevo = crearArchivo(nombre, numPaginas);
        insertarHeap(&heap,nuevo);
        heapify(&heap);

        printf("\nHeap Después: \n");
        imprimirHeap(heap);
        printf("\n");
        imprimirArbol(heap.arbol);
        printf("\n\n");
        break;
      }
      case 3:
        printf("\nHeap Antes: \n");
    	imprimirHeap(heap);
    	printf("\n");
    	imprimirArbol(heap.arbol);
        printf("\n");

        if(heap.cantidad > 0){
          Archivo *procesado = quitarHeap(&heap);
          printf("Procesando archivo... \n");
          imprimirArchivo(procesado);
          printf("\n");
          free(procesado);
          heapify(&heap);
        }else{
          printf("No hay archivos en la cola de impresión\n");
        }

        printf("\nHeap Después: \n");
        imprimirHeap(heap);
        printf("\n");
        imprimirArbol(heap.arbol);
        printf("\n\n");
        break;
      case 4:
        printf("\nHeap Antes: \n");
    	imprimirHeap(heap);
    	printf("\n");
    	imprimirArbol(heap.arbol);
        printf("\n");

        if(heap.cantidad == 0){
          printf("No hay archivos a eliminar\n");
          break;
        }
        printf("Archivos en cola de impresión: \n");
        for(int i = 0; i < heap.cantidad; i++){
          printf("[%d] ",i);
          heap.imprimir(heap.arr[i]->dato);
          printf("\n");
        }

        //int pos = inputEntero("\nIngresa el indice del archivo a eliminar: ", &pos);
        int pos = 0;
        printf("Ingresa el indice del archivo a eliminar: ");
        scanf("%d", &pos);
        eliminarHeapPos(&heap,pos);

        printf("\nHeap Después: \n");
        imprimirHeap(heap);
        printf("\n");
        imprimirArbol(heap.arbol);
        printf("\n\n");
        break;
      case 5:
        printf("\nHeap Antes: \n");
    	imprimirHeap(heap);
    	printf("\n");
    	imprimirArbol(heap.arbol);
        printf("\n");

        vaciarHeap(&heap);

        printf("\nHeap Después: \n");
        imprimirHeap(heap);
        printf("\n");
        imprimirArbol(heap.arbol);
        printf("\n\n");
        break;
      case 6:
        /*
        printf("\nHeap Antes: \n");
    	imprimirHeap(heap);
    	printf("\n");
    	imprimirArbol(heap.arbol);
        printf("\n");

         */
        for(int i = 0; i < heap.cantidad; i++){
          free(heap.arr[i]->dato);
          free(heap.arr[i]);
        }
        free(heap.arr);
        printf("Hasta Luego !\n");

        /*
        printf("\nHeap Después: \n");
        imprimirHeap(heap);
        printf("\n");
        imprimirArbol(heap.arbol);
        printf("\n\n");
        free(heap.arr);

         */
        break;

      default:
        //printf("Opción no valida\n");
        break;
    }
  }while(opcion != 6);
  /*
	Heap heap = inicializarHeap( &imprimirEntero,&compararEntero);
	int arr[] = {3, 5, 8, 10, 17, 11, 13, 19, 22, 24, 29};
	int cant = sizeof(arr)/sizeof(int);
	int i;
	for( i = 0 ; i < cant ; i++)
	{
		insertarHeap(&heap,&arr[i]);
	}
	//CAMBIAR A HEAP MINIMO
	heap.tipo = MIN;
	heapify(&heap);
	printf("\n");
	imprimirHeap(heap);	
	printf("\n");
	imprimirArbol(heap.arbol);
	//CAMBIAR A HEAP MAXIMO
	heap.tipo = MAX;
	heapify(&heap);
	printf("\n");
	imprimirHeap(heap);
	printf("\n");
	imprimirArbol(heap.arbol);
	
	printf("\n");
	while( heap.cantidad )
	{
		void *x = quitarHeap(&heap);
		imprimirEntero(x);
	}
	
	printf("\n\n\n");
	
	
	*/
	return 0;
}


void imprimirEntero(void *a)
{
	int *aa=a;
	printf(" %d ",*aa);
}

int compararEntero(void*a,void*b)
{
	int *aa = a,*bb=b;
	if(*aa==*bb)return 0;
	else if(*aa<*bb)return -1;
	else return 1;
}

//Crear Archivo
Archivo *crearArchivo(char *nombre, int numPaginas){
  Archivo *nuevo = calloc(1,sizeof(Archivo));
  strcpy(nuevo->nombre,nombre);  //copiar nombre
  nuevo->numPaginas = numPaginas;  //copiar num de páginas
  return nuevo;
}

//Comparar Archivo
int compararArchivo(void *a, void *b){
  Archivo *A = a;
  Archivo *B = b;

  //Si min-heap
  if(A->numPaginas == B->numPaginas)
    return 0;
  else if(A->numPaginas < B->numPaginas)
    return -1;
  else
    return 1;

}

//Imprimir archivo
void imprimirArchivo(void *a){
  Archivo *archivo = a;
  printf("%s (%d páginas) | ", archivo->nombre, archivo->numPaginas);
}


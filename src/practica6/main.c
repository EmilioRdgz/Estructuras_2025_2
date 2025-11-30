
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <time.h>
#include "../captura/captura.h"
#include "../Estructuras/arbol/arbol.h"
#include "../Estructuras/nodos/nodoarbol.h"
#include "../Estructuras/pila/pila.h"

int* crearEntero(int);
int compararEntero(void*,void*);
void imprimirEntero(void*);

//Funciones ÁRBOL
void calcularAltura(NodoA *raiz,int *altura);
int altura(Arbol arbol);
void extraccionDatos(NodoA *raiz,void **datos,int *indice);
void equilibrar(Arbol *arbol);

//Funciones Nuevas
void* buscarArbol(Arbol arbol,void *dato);
NodoA* nodoMinimo(NodoA *n);
NodoA* eliminar(NodoA *nodo,void *dato, int (*comparar)(void*,void*), void (*liberar)(void*), int *eliminado);
int eliminarDatoArbol(Arbol *arbol,void *dato);
int compararArboles(NodoA *raizA, NodoA *raizB, int(*comparar) (void*, void*));
int compararDatosArbol(Arbol A, Arbol B, int (*comparar) (void*, void*));
void insertarEquilibrado(Arbol *arbol, void **datos, int inicio, int fin);
void equilibrar(Arbol *arbol);
void eliminarArbol(Arbol *arbol);

void myprintf(char *formato,...);

int main(void)
{
	int opcion;
	int *dato;
	int valor;
    Arbol arbolA, arbolB;

    arbolA.raiz = NULL;
    arbolA.cantidad = 0;
    arbolA.imprimir = &imprimirEntero;
    arbolA.comparar = &compararEntero;
    arbolA.liberar = free;

	arbolB.raiz = NULL;
	arbolB.cantidad = 0;
	arbolB.imprimir = &imprimirEntero;
	arbolB.comparar = &compararEntero;
	arbolB.liberar = free;


	do{
		printf("\n--- MENU ---\n");
		printf("1. Insertar en Árbol A\n");
		printf("2. Insertar en Árbol B\n");
        printf("3. Comparar Árboles \n");
        printf("4. Eliminar datos en Árbol A \n");
		printf("5. Eliminar datos en Árbol B \n");
        printf("6. Calcular Altura\n");
        printf("7. Equilibrar árboles\n");
        printf("8. Imprimir árboles \n");
        printf("9. Salir\n");
		printf("Elige: ");
		scanf("%d", &opcion);
		clear_buffer();

		switch (opcion){
			case 1:
                printf("Dato a insertar en Árbol A: ");
                scanf("%d", &valor);
                dato = malloc(sizeof(int));
                *dato = valor;
                insertarArbol(&arbolA, dato);
                break;
			case 2:
				printf("Dato a insertar en Árbol B: ");
				scanf("%d", &valor);
				dato = malloc(sizeof(int));
				*dato = valor;
				insertarArbol(&arbolB, dato);
			break;
			case 3:{
				int igualesEstructura = compararArboles(arbolA.raiz, arbolB.raiz, arbolA.comparar);
				int igualesDatos = compararDatosArbol(arbolA, arbolB, arbolA.comparar);
                if(igualesEstructura && igualesDatos)
                  printf("\nLos arboles son iguales en Estructura y datos \n");
                else if(!igualesEstructura && igualesDatos)
                  printf("\nLos arboles son iguales en datos\n");
                else
                  printf("Los arboles son diferentes\n");
				break;
			}
            case 4:
            	printf("Dato a eliminar en Árbol A: ");
				scanf("%d", &valor);
				if(eliminarDatoArbol(&arbolA, &valor))
					printf("Dato eliminado del Árbol A.\n");
				else
					printf("Dato no encontrado en Árbol A.\n");
				break;
            case 5:
            	printf("Dato a eliminar en Árbol B: ");
				scanf("%d", &valor);
				if(eliminarDatoArbol(&arbolB, &valor))
					printf("Dato eliminado del Árbol B.\n");
				else
					printf("Dato no encontrado en Árbol B.\n");
				break;
            case 6:
              printf("Altura de Árbol A: %d\n", altura(arbolA));
              printf("Altura de Árbol B: %d\n", altura(arbolB));
              break;
            case 7:
              equilibrar(&arbolA);
              equilibrar(&arbolB);
              printf("Árboles equilibrados correctamente.\n");
              break;
            case 8:
              printf("Árbol A: \n");
              imprimirArbol(arbolA);
              printf("\n");
              printf("Árbol B: \n");
              imprimirArbol(arbolB);
              break;
            case 9:
              printf("Hasta Luego\n");
              break;
            default:
              printf("Opción incorrecta.\n");
		}
	}while(opcion != 9);

    //eliminarArbol(&arbolA);
    //eliminarArbol(&arbolB);

    return 0;
}


int* crearEntero(int dato)
{
	int *nuevo = malloc(sizeof(int));
	*nuevo = dato;
	return nuevo;
}

int compararEntero(void*a,void*b)
{
	int *aa = a,*bb=b;
	if(*aa==*bb)return 0;
	else if(*aa<*bb)return -1;
	else return 1;
}

void imprimirEntero(void *a)
{
	int *aa=a;
	printf("%d",*aa);
}

/*
void myprintf(char *formato,...)
{
	va_list args;
    va_start(args, strlen(formato));
	while(*formato!='\0')
	{
		switch(*formato)
		{
			case 'd':
				printf("%d",(int)va_arg(args, int));
				break;
			case 'c':
				printf("%c",(char)va_arg(args, char));				
				break;
		}
		formato++;
	}
	va_end(args);
}
*/



void calcularAltura(NodoA *raiz,int *altura)
{
	int izquierdo,derecho;
	izquierdo = derecho = *altura;
	if(raiz->izq && izquierdo++ )
		calcularAltura(raiz->izq,&izquierdo);
	if(raiz->dch && derecho++ )
		calcularAltura(raiz->dch,&derecho);
	*altura = (izquierdo>derecho) ? izquierdo:derecho;
}

int altura(Arbol arbol)
{
	if(!arbol.raiz)
		return 0;
	int altura = 1;
	calcularAltura(arbol.raiz,&altura);
	return altura;
}

void extraccionDatos(NodoA *raiz,void **datos,int *indice)
{
	if(!raiz)
		return;
	extraccionDatos(raiz->izq,datos,indice);
	datos[*indice] = raiz->dato;
	(*indice)++;
	extraccionDatos(raiz->dch,datos,indice);

}
/*
void equilibrar(Arbol *arbol)
{
	void (*liberar)(void*) = arbol->liberar;
	arbol->liberar = NULL;
	int indice = 0;
	int cantidad = arbol->cantidad;
	int *indices = calloc(cantidad,sizeof(int));
	void **datos = calloc(cantidad,sizeof(void*));

	extraccionDatos(arbol->raiz,datos,&indice);

	eliminarArbol(arbol);
	arbol->liberar = liberar;
	//ARBOL VACIO
	//DATOS EN EL ARREGLO
	//HACER INSERCIONES EN EL ARBOL PARA QUE
	//QUEDE EQUILIBRARDO

	//SACAR INDICES Y COLOCARLOS EN EL ARREGLO DE INDICES

	for( int i=0; i<cantidad ; i++)
	{
		insertarArbol(arbol, datos[ indices[i] ] );
	}
	free(datos);
	free(indices);
}
*/

//Función para encontrar dato dentro del Arbol
void* buscarArbol(Arbol arbol, void *dato){
	NodoA *actual = arbol.raiz;
	int comparar;

	while(actual != NULL){
		comparar = arbol.comparar(dato,actual->dato);
		if(comparar == 0)
			return actual->dato; //si función de comparar regresa 0, entonces se encontró el dato, retornar dato
		else if(comparar < 0)
			actual = actual->izq; // ir a la izquierda (dato actual es más grande que el que se esta buscando)
		else
			actual = actual->dch; //ir a la derecha (dato actual es menor al que se esta buscando)
	}
	return NULL; // si llega aquí, entonces no se encontro

}

//Función para encontrar nodo más a la izquierda
NodoA* nodoMinimo(NodoA *n){
	if(n == NULL)
		return NULL; //verificar si árbol no esta vacio

	//recorrer a la izquiera hasta llegar al valor más a la izq
	while (n->izq != NULL){
		n = n->izq;
	}
	return n; // retornar el valor
}

//Función recursiva que devuelve el nuevo nodo de la raíz, si es que el dato borrado era raiz.
NodoA* eliminar(NodoA *nodo,void *dato, int (*comparar)(void*,void*), void (*liberar)(void*), int *eliminado){

  int comp;

  if(nodo == NULL)
    return NULL;

  comp = comparar(dato,nodo->dato);

  if(comp < 0)
    //el dato esta a la izq, seguir comparando
    nodo->izq = eliminar(nodo->izq,dato,comparar,liberar,eliminado);
  else if(comp > 0)
    //el dato esta a la dch, seguir comparando
    nodo->dch = eliminar(nodo->dch,dato,comparar,liberar,eliminado);
  else{
    //se encontro el dato a eliminar
    *eliminado = 1;

    //Si no tiene hijos (hoja)
    if(nodo->izq == NULL && nodo->dch == NULL){
      if(liberar){
      	liberar(nodo->dato);
      }
      free(nodo);
      return NULL;
    }

    //Si hay un hijo a la derecha
    if(nodo->izq == NULL){
      NodoA *temp = nodo->dch; //enlazar al hijo
      if(liberar){
      	liberar(nodo->dato);
      }
      free(nodo);
      return temp;
    }

    //Si hay un hijo a la izq
    else if(nodo->dch == NULL){
      NodoA *temp = nodo->izq; //enlzar hijo
      if(liberar){
        liberar(nodo->dato);
      }
      free(nodo);
      return temp;
    }

    //Si tiene 2 hijos
    else{
      NodoA *aux = nodo->dch;
      while(aux->izq != NULL)  //llegar los más izq del subárbol dch
        aux = aux->izq;

      //Copiar el dato al dato actual
      nodo->dato = aux->dato;

      //borrar el sucesor del subárbol dch y NULL para no liberar el dato que acabamos de copiar.
      nodo->dch = eliminar(nodo->dch,aux->dato, comparar, NULL, eliminado);
    }
  }
  //regrear la nueva raíz del sub-árbol
  return nodo;
}


//Función que retona si el valor fue eliminado o no fue eliminado, utilizando la funcón recursiva de eliminar
int eliminarDatoArbol(Arbol *arbol,void *dato){
  int eliminado = 0;
  arbol->raiz = eliminar(arbol->raiz,dato, arbol->comparar, arbol->liberar, &eliminado);
  if(eliminado)
    arbol->cantidad--;
  return eliminado; // si retorna 1 entonces se elimino el dato, si retorna 0 no existe el dato
}


//Función que verifica si dos arbole son iguales en estructura y datos
int compararArboles(NodoA *raizA, NodoA *raizB, int(*comparar) (void*, void*)){
  if(raizA == NULL && raizB == NULL)
    return 1;  //los dos estan vacios
  if(raizA == NULL || raizB == NULL)
    return 0;  //solo hay uno vacio
  if(comparar(raizA->dato, raizB->dato) !=0 )
    return 0;    // los datos son diferentes

  //verificar izq y dch
  return(compararArboles(raizA->izq, raizB->izq, comparar) && compararArboles(raizA->dch, raizB->dch, comparar));
}

int compararDatosArbol(Arbol A, Arbol B, int (*comparar) (void*, void*)){

  // verificar si tienen la misma cantidad de Nodos
  if(A.cantidad != B.cantidad)
      return 0;

  Pila pilaA = {NULL,-1,0,NULL,NULL};
  Pila pilaB = {NULL,-1,0,NULL,NULL};

  NodoA *actualA = A.raiz;
  NodoA *actualB = B.raiz;

  while((!vacia(pilaA) || actualA != NULL) && (!vacia(pilaB) || actualB != NULL)){

      // recorrer a la izq ambos árboles
      while(actualA != NULL){
        push(&pilaA, actualA);
        actualA = actualA->izq;
      }
      while(actualB != NULL){
        push(&pilaB, actualB);
        actualB = actualB->izq;
      }

      //Si una de las dos pilas esta vacia y la otra no entonces no son iguales
      if(vacia(pilaA) != vacia(pilaB))
        return 0;

      // Tomar las cimas para compararlas
      actualA = (NodoA*)pop(&pilaA);
      actualB = (NodoA*)pop(&pilaB);

      //comparar los datos
      if(comparar(actualA->dato, actualB->dato) != 0)
        return 0;

      //recorrer subárboles derechos
      actualA= actualA->dch;
      actualB= actualB->dch;
  }

  // Si termina aquí, entonces son iguales
  if(vacia(pilaA) && vacia(pilaB) && actualA == NULL && actualB == NULL)
    return 1;

  return 0;
}


void insertarEquilibrado(Arbol *arbol, void **datos, int inicio, int fin){

  if(inicio > fin)
    return;

  int medio = (inicio + fin)/2;

  //Insertar el elemento de en medio
  insertarArbol(arbol, datos[medio]);

  //Insertar la mitad izq
  insertarEquilibrado(arbol, datos, inicio, medio - 1);

  //Insertar la mitad dch
  insertarEquilibrado(arbol, datos, medio + 1, fin);
}


void equilibrar(Arbol *arbol){

  void (*liberar)(void*) = arbol->liberar;
  arbol->liberar = NULL;

  int indice = 0;
  int cantidad = arbol->cantidad;
  void **datos = calloc(cantidad,sizeof(void*));

  //extraer datos orden asc
  extraccionDatos(arbol->raiz,datos,&indice);

  //eliminar arbol
  eliminarArbol(arbol);
  arbol->liberar = liberar;

  //insertar datos balanceados
  insertarEquilibrado(arbol,datos,0,cantidad-1);

  free(datos);
}

void eliminarArbol(Arbol *arbol) {
	if (arbol->raiz != NULL) {
		eliminarRecursivo(arbol->raiz, arbol->liberar);
		arbol->raiz = NULL;
		arbol->cantidad = 0;
	}
}



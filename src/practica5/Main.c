#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../captura/captura.h"
#include "../Estructuras/cola/cola.h"

#define NUMERO_CAJAS 5
#define HABILITADA 1
#define DESHABILITADA 0
#define MIN_ARTICULOS 1
#define MAX_ARTICULOS 50

typedef struct
{
	Cola cola;
	int estado;
}Caja;


int* generar_articulos(void);
void imprimirEntero(void*);

void inicializarCajas(Caja cajas[], int numCajas);
void generarClientes(Caja cajas[], int numCajas);
void escanearArticulos(Caja cajas[], int numCajas);
void habilitarCaja(Caja cajas[],int numCajas);
void deshabilitarCaja(Caja cajas[], int numCajas);
void liberarMemoria(Caja cajas[], int numCajas);
void mostrarCajas(Caja cajas[], int numCajas);

int main(void){

  srand(time(NULL));

  Caja cajas[NUMERO_CAJAS];
  int opcion;
  printf("\n");
  //inicializar las 5 cajas
  inicializarCajas(cajas, NUMERO_CAJAS);
  printf("\n");
  //Menú
  do{
      mostrarCajas(cajas, NUMERO_CAJAS);
      printf("\n------ MENÚ PRINCIPAL ------\n");
      printf("[1] Generar clientes\n");
      printf("[2] Escanear artículos\n");
      printf("[3] Habilitar caja\n");
      printf("[4] Deshabilitar caja\n");
      printf("[5] Salir del programa\n");
      printf("Seleccione una opción: ");
      scanf("%d", &opcion);

      switch(opcion){

        case 1:
          generarClientes(cajas, NUMERO_CAJAS);
          break;
        case 2:
          escanearArticulos(cajas, NUMERO_CAJAS);
          break;
        case 3:
          habilitarCaja(cajas, NUMERO_CAJAS);
          break;
        case 4:
          deshabilitarCaja(cajas, NUMERO_CAJAS);
          break;
        case 5:
          liberarMemoria(cajas, NUMERO_CAJAS);
          printf("\nFIN DE PROGRAMA\n");
          break;

        default:
          printf("Opción no válida. Intenta nuevamente\n");
      }
  }while(opcion != 5);

  return 0;
}


int* generar_articulos(void)
{		
	int *articulos = malloc(sizeof(int));
	*articulos = rand() % (MAX_ARTICULOS - MIN_ARTICULOS + 1) + MIN_ARTICULOS;
	return articulos; 
	
}


void imprimirEntero(void *a)
{
	int *aa=a;
	printf("%d",*aa);
}

//Función para crear las 5 cajas
void inicializarCajas(Caja cajas[], int numCajas){
  for(int i = 0; i < numCajas; i++){
  cajas[i].cola = inicializarCola(); //iniciar cola en cada caja
  cajas[i].cola.imprimir = imprimirEntero;
  cajas[i].cola.liberar = free;
  cajas[i].estado = 1;

  printf("Caja [%d] inicializada y habilitada\n",i + 1);
  }
}

void generarClientes(Caja cajas[], int numCajas){
  for(int i = 0; i < numCajas; i++){
    //si la caja esta habilitada y tiene espacio
    if(cajas[i].estado && !llenaC(cajas[i].cola)){
      int *articulos = (int*)malloc(sizeof(int));
      *articulos = rand() % 50 + 1;  //generar articulos entre 1 a 50

      //insertar el cliente al final
      enqueue(&cajas[i].cola, articulos);
      printf("Nuevo cliente agregado a la caja [%d] con %d articulos\n",i+1, *articulos);
    }
    else{
      printf("Caja [%d] no esta habilitado o esta llena.\n",i + 1);
    }
  }
}

void escanearArticulos(Caja cajas[], int numCajas){
  for(int i = 0; i < numCajas; i++){
    //Si caja esta habilitad y no esta vacía
    if(cajas[i].estado && !vaciaC(cajas[i].cola)){

      //peek al primer cliente
      int *articulos = (int*) peekC(cajas[i].cola);

      //escanear artículo (eliminar articulo)
      (*articulos)--;
      printf("Caja [%d]: Escaneando... Cliente con %d articulo restantes\n", i + 1, *articulos);

      //Si termino la cant de articulos entonce eliminarlo de la cola
      if(*articulos <= 0 ){
        int *cliente = (int*)dequeue(&cajas[i].cola);
        free(cliente);
        printf("Cliente atentdido y eliminado de la caja [%d]\n", i + 1);
      }
    }
    else if(vaciaC(cajas[i].cola)){  //si caja vacia
      printf("Caja [%d] vacia\n" ,i + 1);
    }
    else if(!cajas[i].estado){ // si caja deshabilitada
      printf("Caja [%d] deshabilitada, No hay atencion\n", i + 1);
    }
  }

}

void habilitarCaja(Caja cajas[],int numCajas){

  int num;
  printf("\nIngrese el número de la caja a habilitar (1-%d): ", numCajas);
  scanf("%d",&num);

  if(num < 1 || num > numCajas){
    printf("Numero de caja equivocado\n");
    return;
  }

  if(cajas[num - 1].estado == 1){
    printf("La caja [%d] ya esta habilitada\n",num);
  }
  else{
    cajas[num - 1].estado = 1;
    printf("La caja [%d] ha sifo habilitada\n", num);
  }
}


void deshabilitarCaja(Caja cajas[], int numCajas){
	int num;
	printf("\nIngrese el número de la caja a deshabilitar (1-%d): ", numCajas);
	scanf("%d", &num);

	if (num < 1 || num > numCajas)
	{
		printf("Número de caja inválido.\n");
		return;
	}

	if (cajas[num - 1].estado == 0)
	{
		printf("La caja [%d] ya está deshabilitada.\n", num);
	}
	else
	{
		cajas[num - 1].estado = 0;
		printf("La caja [%d] ha sido deshabilitada.\n", num);
	}
}

void liberarMemoria(Caja cajas[], int numCajas){
  for(int i = 0; i < numCajas; i++){

    // liberar los clientes de la cola
    eliminarCola(&cajas[i].cola);
    //printf("Memoria liberada de la caja [%d]\n", i + 1);
  }
  printf("\n");
}


void mostrarCajas(Caja cajas[], int numCajas){

  for(int i = 0; i < numCajas; i++){
    printf("[%d] ", i + 1);

    if(!cajas[i].estado){
      printf("Caja deshabilitada\n");
      continue;
    }

    if(vaciaC(cajas[i].cola)){
      printf("vacía");
    }

    Nodo *actual = cajas[i].cola.inicio;
    int contador = 0;

    //recorrer la cola mientras haya cliente y menor a 10
    while(actual != NULL && contador < 10){
      //imprimir num de articulos
      int *dato = (int*)actual->dato;
      printf("%d ",*dato);

      //avanzar al sig cliente
      actual = actual->sig;

      if(actual != NULL && contador < 9){
        printf(" -> ");
      }
      contador++;
    }
    printf("\n");
  }

}

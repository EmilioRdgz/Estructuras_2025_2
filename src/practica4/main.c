#include <stdio.h>
#include <stdlib.h>
#include "../Estructuras/pila/pila.h"

#define ARRIBA 1
#define ABAJO 2
#define DERECHA 4
#define IZQUIERDA 8

#define REN 27
#define COL 26

typedef struct
{
	int x,y;
}Coordenada;


unsigned char ** crear_laberinto(void);
void imprimirLab(unsigned char **lab);


Coordenada* crearCoordenada(int x,int y);
int compararCoordenada(void *a,void *b);
void imprimirCoordenada(void *dato);

unsigned char alternativas(unsigned char **lab,Coordenada *coordenada);
void imprimirPosibles(unsigned char);


//Nuevas Funciones
Coordenada* encontrarCoordenadaInicio(unsigned char **lab, char cordenadaInicio);
Pila iniciarPila(Coordenada *inicio);
Coordenada* peek(Pila *pila);
unsigned char obtenerCaminos(unsigned char **lab, Pila *pilaRecorrido);
Coordenada* siguienteMovimiento(unsigned char **lab, Coordenada *actual, unsigned char caminos);
void resolverLaberinto(unsigned char **lab, Pila *pilaRecorrido);
void liberarLaberinto(unsigned char **lab);

int main(void)
{
    int i,j;

    unsigned char **lab;
    Coordenada *inicio;
    Pila pilaRecorrido;

    lab = crear_laberinto();
    printf("\nLaberinto Original:");
    lab [20][8] = 'B';
	imprimirLab(lab);
    printf("\n\n");

    //Encontrar coordenada A para iniciar
    inicio = encontrarCoordenadaInicio(lab, 'A');
    if(inicio == NULL){
      printf("\nLaberinto no tiene coordenada A\n");
      return 1;
    }

    //Iniciar pila en la coordenada 'A'
    pilaRecorrido = iniciarPila(inicio);

    //Encontrar coordenada 'B'
    resolverLaberinto(lab,&pilaRecorrido);

    //Imprimir coordenadas de punto de inicio a fin
    printf("Recorrido Final:");
    imprimirPilaInvertida(pilaRecorrido);

    //Imprimir el laberinto final
    printf("\n\nLaberinto Final: ");
    imprimirLab(lab);

    //Liberar la memoria
	liberarLaberinto(lab);
    free(inicio);




    return 0;
}



unsigned char ** crear_laberinto(void)
{
    int i,j;
    char *tmp =
"xxxxxxxxxxxxxxxxxxxxxxxxxx\
xAx*****x****************x\
x*xxx*x*x*xxxxxxxxx*xxxx*x\
x*****x*x******x**x****x*x\
xxxxxxx*xxxxxx*xx*xxxx*xxx\
x*****x*x****x*x****xx***x\
x*xxx*x*xxxx*x*x*xxxxx*x*x\
x***x*x****x*****xxxxxxx*x\
xxx*x*xxxx*xxxxxxx****x**x\
x*x*x***xx****xx***xx*x*xx\
x*x*x*x*xxxxx**x*xxxx*x**x\
x*x*x*x***x*xx*x****x*xx*x\
x*x*x*xxx****x*x*xx*x****x\
x*x*x*xxxxxxxx*x**x*xxxx*x\
x***x********x*xx*x*x****x\
x*xxxxxxxxxx*x**xxx*x*xxxx\
x***x******x**x*****x**x*x\
xxx*x*xxxxxxx*xxxxxxxx*x*x\
x*x*x*******x****xx****x*x\
x*x*x*xxxxx*xxxx*xx*xxxx*x\
x*x*x****xx***x**xx*x****x\
x*x*xxxxxxx*x**x*xx*x*x*xx\
x*x*********xx*x*xx*xxx*xx\
x*xxxxxxxxxxx**x*********x\
x***x***x***x*xxxxxxxxxx*x\
x*x***x***x**************x\
xxxxxxxxxxxxxxxxxxxxxxxxxx\0";



    unsigned char **lab;
    lab = (unsigned char**) malloc(sizeof(unsigned char *)*REN);

    for(i=0;i<REN;i++)
    {
        lab[i] = (unsigned char*) malloc(sizeof(unsigned char)*COL);
    }

    for(i=0;i<REN;i++)
    {
        //printf("\n\r");
        for(j=0;j<COL;j++)
        {
            //printf("%c",tmp[(i*COL)+j]);
            lab[i][j] = tmp[(i*COL)+j];
        }
    }
    return lab;
}


void imprimirLab(unsigned char **lab)
{
	int i,j;
	for(i=0;i<REN;i++)
    {
        printf("\n\r");
        for(j=0;j<COL;j++)
        {
            if(lab[i][j]=='*')printf("  ");
			else printf("%c ",lab[i][j]);
        }
    }
}

Coordenada* crearCoordenada(int x,int y)
{
	Coordenada *nueva = malloc(sizeof(Coordenada));
	nueva->x=x;
	nueva->y=y;
	return nueva;
}


void imprimirCoordenada(void *dato)
{
	Coordenada *a = dato;
	printf("[%-2d,%-2d]", a->x,a->y);
}

int compararCoordenada(void *a,void *b)
{
	Coordenada *aa=a,*bb=b;
	if(aa->x==bb->x && aa->y==bb->y)
		return 1;
	return 0;
}


unsigned char alternativas(unsigned char **lab,Coordenada *coordenada)
{
	unsigned char caracter;
	unsigned char banderas;
	banderas = 0;
	//EVALUAR ALTERNATIVA Y REGISTRAR POSIBILIDAD DE MOVIMIENTO
	//EN UN REGISTRO DE BITS/BANDERAS
	caracter = lab[coordenada->x-1][coordenada->y];
	if(caracter!='x' && caracter!='.' && caracter!='o' && caracter!='A')
		banderas |= ARRIBA;

	caracter = lab[coordenada->x+1][coordenada->y];
	if(caracter!='x' && caracter!='.' && caracter!='o' && caracter!='A')
		banderas |= ABAJO;

	caracter = lab[coordenada->x][coordenada->y+1];
	if(caracter!='x' && caracter!='.' && caracter!='o' && caracter!='A')
		banderas |= DERECHA;

	caracter = lab[coordenada->x][coordenada->y-1];
	if(caracter!='x' && caracter!='.' && caracter!='o' && caracter!='A')
		banderas |= IZQUIERDA;
	return banderas;
}


void imprimirPosibles(unsigned char caminos)
{
	if(caminos&ARRIBA)
		printf("\n ARRIBA");
	if(caminos&ABAJO)
		printf("\n ABAJO");
	if(caminos&IZQUIERDA)
		printf("\n IZQUIERDA");
	if(caminos&DERECHA)
		printf("\n DERECHA");
}

//Encontrar coordenada de Inicio
Coordenada* encontrarCoordenadaInicio(unsigned char **lab, char coordenadaObjetivo){

  int i,j;
  for(i = 0; i < REN; i++){
    for(j = 0; j < COL; j++){
      if(lab[i][j] == coordenadaObjetivo){
        Coordenada *encontrada = crearCoordenada(i,j);
        return encontrada;
      }
    }
  }
  return NULL;

}

//Inicializar pila de Recorrido y hacer push a la posición de Inicio (coordenada A)
Pila iniciarPila(Coordenada *inicio){

  //Crear pila para recorrido del mismo tam del laberinto
  Pila pilaRecorrido = {NULL, REN * COL, 0, imprimirCoordenada, free};
  //Push a la primera coordenada "A"
  push(&pilaRecorrido, crearCoordenada(inicio->x,inicio->y));

  return pilaRecorrido;

}

//Función Peek
Coordenada* peek(Pila *pila){
  Coordenada *dato;
  //si pila vacia regresa nulo
  if(vacia(*pila)){
    return NULL;
  }
  //Pop a la cima de la pila y castear el dato.
  dato = (Coordenada*) pop(pila);

  //Regresar el dato a la cima
  if(dato != NULL){
    push(pila,dato);
  }

  return dato; //regresar el dato de la cima

}

//Función para saber los caminos posibles
unsigned char obtenerCaminos(unsigned char **lab, Pila *pilaRecorrido){
  Coordenada *actual;
  unsigned char caminos;

  actual = peek(pilaRecorrido);
  if(actual == NULL){
    return 0;
  }

  //Evaluar las direcciones posibles
  caminos = alternativas(lab, actual);

  //Devolver las bandera de las direcciones válidas
  return caminos;
}

//Función que analiza el siguiente movimiento
Coordenada* siguienteMovimiento(unsigned char **lab, Coordenada *actual, unsigned char caminos){
  int nX = actual->x;
  int nY = actual->y;

  //Preferencia: Derecha, Izq, Abajo, Arriba
  //Intentar derecha
  if(caminos & DERECHA){
    nY = actual->y + 1;
    return crearCoordenada(nX,nY);
  }

  //Intentar Izq
  if(caminos & IZQUIERDA){
    nY = actual->y - 1;
    return crearCoordenada(nX,nY);
  }

  //Intentar Abajo
  if(caminos & ABAJO){
    nX = actual->x + 1;
    return crearCoordenada(nX,nY);
  }

  //Intentar Arriba
  if(caminos & ARRIBA){
    nX = actual->x - 1;
    return crearCoordenada(nX,nY);
  }

  //Si no hay caminos disponibles, regresar Nulo
  return NULL;
}

//Función para recorrer el laberindo utilizando la pila
void resolverLaberinto(unsigned char **lab, Pila *pilaRecorrido){

  Coordenada *actual;
  Coordenada *sig;
  unsigned char caminos;

  //Mientras existan coordenadas en la pilaRecorrido
  while(!vacia(*pilaRecorrido)){

    //peek a cima de la pila
    actual = peek(pilaRecorrido);

    //Si llegamos a 'B' entonces encontramos el camino
    if(lab[actual->x][actual->y] == 'B'){
      printf("\nFinal Encontrado en ");
      imprimirCoordenada(actual);
      printf("\n");
      return;
    }

    // Caminos posibles desde la posición actual
    caminos = obtenerCaminos(lab, pilaRecorrido);

    //Si hay caminos posibles
    if(caminos != 0){
      //Marcar la posición actual como visitada
      if(lab[actual->x][actual->y] != 'A'){
        lab[actual->x][actual->y] = '.';  //Dejar migaja
      }

      //Elegir la siguiente coordenada
      sig = siguienteMovimiento(lab, actual, caminos);

      //Push de la nueva coordenada a la pila, avanzar a la coordenada regresada en sig, según la jerarquía indicada
      push(pilaRecorrido, sig);
    }
    else{
      //Si no hay caminos, marcar como 'o' y retroceder
      if(lab[actual->x][actual->y] != 'A'){
        lab[actual->x][actual->y] = 'o'; //marcar como sin salida
      }

      //Retroceder
      Coordenada *temp = pop(pilaRecorrido);
      free(temp);
    }
  }

  //Si sale del ciclo entonces no se encontro 'B'
  printf("\nNo se encontró el camino a 'B'.\n");
}

void liberarLaberinto(unsigned char **lab){
    int i;
    for (i = 0; i < REN; i++)
    {
        free(lab[i]);  // liberar cada fila
    }
    free(lab);  // liberar el arreglo
}

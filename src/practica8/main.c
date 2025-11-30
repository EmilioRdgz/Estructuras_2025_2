#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include "../captura/captura.h"
#include "../Estructuras/pila/pila.h"
#include "../Estructuras/cola/cola.h"
#include "../Estructuras/arbol/arbol.h"
#include "../Estructuras/nodos/nodoarbol.h"
#define longitud 30

typedef struct
{
    char caracter;
    float valor;
}Dato;


Dato* crearDato(char caracter);
void imprimirDato(void *dato);
Cola* expresionACola(char *expresion);
void imprimirEntero(void *a);

//Prototipos Práctica 7
void imprimirCadena(void *a);
void liberarCadena(void *a);
int estaBalanceada(const char* cadena);
char* infixPostfix(char *expresion);
int contarParentesis(char *expresion);
int esOperador(char c);
int esOperando(char c);
int precedencia(char Operador);
int compararPrecedencia(void *a, void *b);
NodoA* ConstruirArbol(char *postfix, Cola *cola);
void eliminarArbol(Arbol *arbol);
int validarPostfix(const char *postfix);

char ultimaExpresion[longitud];
char ultimaExpresionValida[longitud] = "";
//NodoA Raiz para la creación del árbol mediante la pila y el arbol que se usará.
NodoA *raizGlobal = NULL;
Arbol arbolGlobal;
int expresionValida = 0; //bandera para conocer si es válida la expresión o no

void capturarValores(Cola *cola);
float evaluarArbol(NodoA *nodo);


int main(void)
{
    int opcion;
    do{
        printf("\nEXPRESIÓN: %s \n",ultimaExpresionValida);
        printf("[0] CAPTURAR EXPRESIÓN\n");
        printf("[1] IMPRIMIR ÁRBOL DE EXPRESIÓN\n");
        printf("[2] EVALUAR EXPRESIÓN\n");
        printf("[3] TERMINAR PROGRAMA\n");
        scanf("%d", &opcion);
        clear_buffer();

        switch (opcion){
          case 0:
            inputCadena("\nIngresa la expresión: ", ultimaExpresion, longitud);
            char *postfix = infixPostfix(ultimaExpresion);
            if (postfix == NULL || !validarPostfix(postfix)) {
            printf("Expresión no válida.\n");
            free(postfix);
            break;
          }

          //printf("Postfix: %s\n", postfix);
          //Crear Cola con NodosA y luego construir el Arbol con la expresión postfix y la cola
          Cola *cola = expresionACola(postfix);
          raizGlobal = ConstruirArbol(postfix, cola);
          if (!raizGlobal) {
            printf("Error construyendo árbol.\n");
            expresionValida = 0;
            break;
          }

          //inicializar arbol
          arbolGlobal.raiz = raizGlobal;
          arbolGlobal.imprimir = &imprimirDato;
          strcpy(ultimaExpresionValida, ultimaExpresion);
          expresionValida = 1;
          //
          free(postfix);
          break;
            case 1:
              if(!expresionValida || raizGlobal == NULL){
                printf("No hay una expresión válida\n ");
                break;
              }
              printf("\nÁrbol de expresión actual: \n");
              imprimirArbol(arbolGlobal);
            break;
            case 2:
              if(!expresionValida || raizGlobal == NULL){
                printf("No hay expresión válida para evaluar\n");
                break;
              }
              capturarValores(cola);
              float res = evaluarArbol(raizGlobal);
              printf("\nResultado = %.2f \n", res);
              break;
            case 3:
              printf("Saliendo del programa...\n");
              if (expresionValida && raizGlobal != NULL) {
                eliminarArbol(&arbolGlobal);
                eliminarCola(cola);
                free(cola);
              }
          break;
            default:
              printf("Opción no válida\n ");
              break;
        }
    }while(opcion!=3);

/*
    char *postfix = NULL;
    inputCadenaDinamica("\n Captura postfix: ",&postfix,100);
    Cola *colaExpresion = expresionACola(postfix);
    NodoA *nodo = peekC(*colaExpresion);
    Dato* dato = nodo->dato;
    dato->valor = 10;
    printf("\n Cola: ");
    imprimirCola(*colaExpresion);
    printf("\n\n FIN DE PROGRAMA\n");
    return 0;
 */
}


Dato* crearDato(char caracter)
{
    Dato *nuevo = calloc(1,sizeof(Dato));
    nuevo->caracter = caracter;
    return nuevo;
}

void imprimirDato(void *dato)
{
  Dato *i = dato;
  printf(" %c ", i->caracter);
}

int compararCaracteres(void *param_a,void *param_b)
{
    char *datoa  = param_a;
    NodoA *nodob = param_b;
    Dato *datob  = nodob->dato;
    if( *datoa == datob->caracter)
        return 0;
    return 1;
}


Cola* expresionACola(char *expresion)
{
    Cola *cola = calloc(1,sizeof(Cola));
    *cola = inicializarCola();
    cola->imprimir = &imprimirDato;
    int i;
    for( i = 0; expresion[i]!='\0' ; i++)
    {
        NodoA *nodo = buscarC(cola,&expresion[i],&compararCaracteres);
        Dato* dato;
        if(nodo)
            dato = nodo->dato;
        else
            dato = crearDato(expresion[i]);
        NodoA *nodoa = crearNodoA(dato);
        enqueue(cola,nodoa);
    }
    return cola;
}

//Función que recibe la expresión postfix y crea el arbol devolviendo la raiz del árbol
NodoA* ConstruirArbol(char *postfix, Cola *cola){

  Pila pila = {NULL,-1,0,NULL,NULL};

  for(int i = 0; postfix[i] != '\0'; i++){
    char c = postfix[i];

    if (esOperando(c)){

      //Buscar si ya existe el carácter ya existe
      NodoA *nodoExiste = buscarC(cola,&c,&compararCaracteres);

      Dato *dato;
      if(nodoExiste){
        // si dato existe entonces usarlo otra vez
        dato = nodoExiste->dato;
      }else{
        // Si no existe crear nuevo dato y agregarlo a la cola
        dato = crearDato(c);
        NodoA *nodo = crearNodoA(dato);
        enqueue(cola,nodo);
      }

      // Crear nodo del árbol
      NodoA *nodoArbol = crearNodoA(dato);
      push(&pila, nodoArbol);
    }

    else if(esOperador(c)){
      //Verififcar si pila no esta vaci y hacer pop de los últimos dos operandos
      if(vacia(pila))
        return NULL;
      NodoA *dch = pop(&pila);
      if(vacia(pila))
        return NULL;
      NodoA *izq = pop(&pila);

      //Crear el nodo operador y enlazar con los que se acaban de sacar (Operandos)
      Dato *datoOperador = crearDato(c);
      NodoA *nodoOperador = crearNodoA(datoOperador);
      nodoOperador->izq = izq;
      nodoOperador->dch = dch;


      //Push a pila el nuevo subárbol
      push(&pila, nodoOperador);
    }
  }

  //Dejar la raíz del árbol en la cima de la pila
  NodoA *raiz = pop(&pila);
  eliminarPila(&pila);
  return raiz;
}

//Funciones para capturar Valores de las variables

void capturarValores(Cola *cola){
  int variableDoble[256] = {0};
  Nodo *actual =  cola->inicio;
  while(actual){
    NodoA *nodo = (NodoA*) actual->dato;
    Dato *dato = (Dato*)nodo->dato;
    char c = dato->caracter;
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
      if(!variableDoble[(int)c]){
        printf("Ingres el valor para %c: ", c);
        scanf("%f", &dato->valor);
        variableDoble[(int)c] = 1; //marcar como variable Doble
      }
    }
    actual = actual->sig;
  }
}

float evaluarArbol(NodoA *nodo){
  if(nodo == NULL)
    return 0;

  Dato *dato = (Dato*)nodo->dato;
  if ((dato->caracter >= 'A' && dato->caracter <= 'Z') || (dato->caracter >= 'a' && dato->caracter <= 'z') || (dato->caracter >= '0' && dato->caracter <= '9')) {
    return dato->valor;
  }
  //evaluar los subárboles izq y dch
  float izq = evaluarArbol(nodo->izq);
  float dch = evaluarArbol(nodo->dch);

  //Evaluar dependiendo el operador
  switch(dato->caracter){
    case '+': return izq + dch;
    case '-': return izq - dch;
    case '*': return izq * dch;
    case '/':
      if(dch == 0){
        printf("División entre 0\n");
        return 0;
      }
      return izq / dch;
    case '^':
      return powf(izq, dch);
    default: return 0;
  }
}



void imprimirEntero(void *a)
{
  int *aa=a;
  printf("%d",*aa);
}



//FUNCIONES PRÁCTICA 7

void imprimirCadena(void *a)
{
  char *aa = a;
  printf("%s",aa);

}

void liberarCadena(void *a)
{
  char *aa = a;
  free(aa);
}
//Función de Práctica 3 para verificar si la expresión esta balanceada
int estaBalanceada(const char* cadena){
  Pila pila = {NULL,-1,0,&imprimirCadena,&liberarCadena};

  for(int i = 0; i < strlen(cadena); i++){
    if(cadena[i] == '(' || cadena[i] == '[' || cadena[i] == '{'){
      char *c = malloc(sizeof(char));
      *c = cadena[i];
      push(&pila,c);
    }
    else if (cadena[i] == ')'){
      if(vacia(pila)){
        //printf("UNDERFLOW\n");
        return 0;

      }
      char *cima = pop(&pila);
      if(*cima != '('){
        free(cima);
        return 0;
      }
      free(cima);
    }
    else if (cadena[i] == ']'){
      if(vacia(pila)){
        printf("UNDERFLOW\n");
        return 0;

      }
      char *cima = pop(&pila);
      if(*cima != '['){
        free(cima);
        return 0;
      }
      free(cima);
    }
    else if (cadena[i] == '}'){
      if(vacia(pila)){
        printf("UNDERFLOW\n");
        return 0;
      }
      char *cima = pop(&pila);
      if(*cima != '{'){
        free(cima);
        return 0;
      }
      free(cima);
    }
    else{
      continue; //continuar para verificar toda la cadena
    }
  }
  if(vacia(pila)){
    return 1;
  }
  else{
    return 0;
  }
}
int validarPostfix(const char *postfix) {
  int pila = 0; // cuenta de operandos disponibles
  for (int i = 0; postfix[i] != '\0'; i++) {
    char c = postfix[i];
    if (esOperando(c)) {
      pila++;
    }
    else if (esOperador(c)) {
      if (pila < 2) return 0; // operador sin 2 operandos
      pila--; // usa dos, deja uno
    }
  }
  return (pila == 1); // expresión correcta si queda un solo resultado final
}


//Función principal para cambiar expresión de Infix a Postfix
char* infixPostfix(char *expresion){
  if(!estaBalanceada(expresion)){
    //printf("La expresión no esta balanceada\n");
    return NULL;
  }

  int longitudExpresion = strlen(expresion);
  int cantidadParentesis = contarParentesis(expresion);
  int longitudReserva = longitudExpresion - cantidadParentesis;
  int i,j = 0;
  char *postfix = calloc(longitudReserva + 1, sizeof(char));

  //Convertir Expresión A Postfix
  //Usando Pila
  Pila pila = {NULL,-1,0,&imprimirCadena,&liberarCadena};

  //ciclo para recorrer la expresión
  for(i = 0; expresion[i] != '\0'; i++){
    char c = expresion[i];

    // si es espacio continuar
    if(c == ' ')
      continue;

    // si el caracter actual es un Operando, entonces meter directamente a expresión Postfix
    if(esOperando(c)){
      postfix[j++] = c; // agregar el carácter a la expresión postfix
    }

    else if(c == '('){ // si es parentesís ingresar el parentesís a la pila
      char *nuevo = malloc(sizeof(char));
      *nuevo = c;
      push(&pila,nuevo);
    }

    // si caracter actual es ')' y la pila esta vacía entonces underflow
    else if(c == ')'){
        while(!vacia(pila)){
          // si no esta vacía la pila entonces pop de cima
          char *cima = (char*) pop(&pila);
          if(*cima == '('){ // si la cima es '(' entonces liberar la cima y parar para no incluir parentesis.
            free(cima);
            break;
          }else{ // de lo contrario agregar la cima a la expresión postfix y liberar
            postfix[j++] = *(cima);
            free(cima);
          }
        }

    }

    else if(esOperador(c)){
      while(!vacia(pila)){
        //hacer peek de la pila
        char *peek = (char*) pila.cima->dato;
        if(*peek == '(')
          break;
        //usar la función de comparar el dato actual con la cima de la pila
        //para saber que operador es mayor y decidir si sacar de la pila o meter
        int comp = compararPrecedencia(&c, peek);
        if(comp <= 0){
          char *nuevoMayor = (char*)pop(&pila);
          postfix[j++] = *nuevoMayor;
          free(nuevoMayor);
        }
        else
          break;
      }
      //Meter a la pila el caracter actual
      char *nuevo = malloc(sizeof(char));
      *nuevo = c; //asignar a nuevo el dato del caracter actual
      push(&pila,nuevo);
    }

    // de lo contrario no es un cáracter valido
    else{
      printf("Carácter Invalido: %c\n",c);
      free(postfix);
      eliminarPila(&pila);
      return NULL;
    }
  }

  //si quedan operadores al final en la pila entonces sacarlos
  while(!vacia(pila)){
    char *operadores = (char*) pop(&pila);
    postfix[j++] = *operadores;
    free(operadores);
  }

  postfix[j] = '\0'; //marcar el ultimo caracter como caracter nulo para terminar la cadena
  eliminarPila(&pila);
  return postfix;
}


//Función para contar cantidad de parentesís
int contarParentesis(char *expresion){
  int cantidadParentesis = 0;
  for(int i = 0; expresion[i] != '\0'; i++){
    if(expresion[i] == '(' || expresion[i] == ')'){
      cantidadParentesis++;
    }
  }
  return cantidadParentesis;
}

//Función para verificar si es Operador válido
int esOperador(char c){
  if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^'){
    return 1;
  }
  else{
    return 0;
  }
}

//Función para verificar si es Operando Válido
int esOperando(char c){
  if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
    return 1;
  }
  else{
    return 0;
  }
}

//Función para conocer la precedencia del operador
int precedencia(char c){
  switch(c){
    case '+': case '-':
      return 1;
    case '*': case '/':
      return 2;
    case '^':
      return 3;
  }
  return 0;
}

//Función para comparar que Operador es mayor
int compararPrecedencia(void *a, void *b){
  char *aa = a, *bb = b;
  if(precedencia(*aa) > precedencia(*bb) ){
    return 1;
  }else if(precedencia(*aa) < precedencia(*bb) ){
    return -1;
  }else return 0;

}

void eliminarArbol(Arbol *arbol) {
  if (arbol->raiz != NULL) {
    eliminarRecursivo(arbol->raiz, arbol->liberar);
    arbol->raiz = NULL;
    arbol->cantidad = 0;
  }
}

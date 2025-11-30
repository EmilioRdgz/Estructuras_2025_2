#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../captura/captura.h"
#include "../Estructuras/pila/pila.h"
#define longitud 30

void imprimirCadena(void *a);
void liberarCadena(void *a);
int estaBalanceada(const char* cadena);
char* infixPostfix(char *expresion);
int contarParentesis(char *expresion);
int esOperador(char c);
int esOperando(char c);
int precedencia(char Operador);
int compararPrecedencia(void *a, void *b);

int main(void){
  int opcion;
  char expresion[longitud];

  do{
    printf("\n--- MENU ---\n");
    printf("1. Infix a Postfix\n");
    printf("2. Salir\n");
    printf("Elige: ");
    scanf("%d", &opcion);
    clear_buffer();

    switch (opcion){
      case 1:
        inputCadena("Ingresa la cadena a convertir de Infix a Postfix: \n", expresion,longitud);
        char *res = infixPostfix(expresion);
        if(res != NULL){
          printf("Expresión Postfix: %s\n", res);
        }else
          printf("Error, expresión no valida\n");
        break;
      case 2:
        printf("Saliendo del programa\n");
        break;
    }
  }while(opcion!=2);
}


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

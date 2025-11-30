//INCLUDES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../captura/captura.h"
#include "../Estructuras/pila/pila.h"
//FUNCION MAIN

#define LONGITUD 64

void imprimirCadena(void *a);
void liberarCadena(void *a);
int esPalindromo(const char* cadena);
int estaBalanceada(const char* cadena);

int main(void)
{
	int opcion;
    char cadena[LONGITUD];

    do {
        printf("\n--- MENU ---\n");
        printf("1. Verficar si la palabras es palíndromo\n");
        printf("2. Verficar Cadena de paréntesis\n");
        printf("3. Salir\n");
        printf("Elige: ");
        scanf("%d", &opcion);
        clear_buffer();

        switch(opcion) {
            case 1:
              inputCadena("Ingres la cadena a verificar si es palindromo: \n", cadena,LONGITUD);
              if(esPalindromo(cadena)){
                printf("La cadena es palindromo\n");
              }else{
                printf("La cadena no es palindromo\n");
              }
            break;
            case 2:
              inputCadena("Ingrese la cadena de Paréntesis a verificar: \n",cadena,LONGITUD);
              if(estaBalanceada(cadena)){
                printf("La cadena esta balanceada\n");
              }else{
                printf("La cadena no esta balanceada\n");
              }
              break;
        }
    } while(opcion != 3);

    //borrarLista(&lista);
    return 0;

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


int esPalindromo(const char* cadena){
	Pila pila = {NULL,-1,0,&imprimirCadena,&liberarCadena}; //inicializar la pila
	for(int i =0; cadena[i] != '\0'; i++) //recorrer cadena
        {
		if(cadena[i] != ' ') //si la cadena no es un espacio
          {
			char *c = malloc(sizeof(char));  //reservar memoria para el carácter
			*c = tolower(cadena[i]); //convertir el caracter a minúsculs
            push(&pila,c);  //insertar carácter en pila
		  }
		}

    for(int i =0; cadena[i] != '\0'; i++){
      if(cadena[i] != ' '){
        char caracterActual = tolower(cadena[i]); // convertir el carácter actual en minúscula
        char *caracterPila = pop(&pila);  // sacar la cima de la pila
        if(caracterPila == NULL || caracterActual != *caracterPila){  //verificar si pila vacía o si el caracter actual es dieferente al caracter de la pila
          if(caracterPila != NULL){ //si es diferente de nulo
            free(caracterPila);
          }
          eliminarPila(&pila);  // si cumple con lo anterior, entonces no es palíndromo, limpiar la pila y regresar un 0
          return 0;
        }
        free(caracterPila);   //liberar los carácteres que si coinciden
      }
    }
    eliminarPila(&pila);  // si no hubo problemas entonces es palíndromo y liberar la pila para evitar fugas de memoria
    return 1;

}

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
        printf("UNDERFLOW\n");
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
      return 0;
    }
  }
  if(vacia(pila)){
    return 1;
  }
  else{
    return 0;
  }
}


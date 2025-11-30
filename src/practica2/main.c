#include <stdio.h>

#include "../Estructuras/listadoble/listadoble.h"
#include "Alumno.h"
#include "../captura/captura.h"

#define LONGITUD 5


int main(void)
{
  ListaD lista;
  lista.inicio = NULL;
  lista.fin =  NULL;
  lista.cant = 0;
  lista.comparar = &compararPorMatricula;
  lista.imprimir = &imprimirAlumno;



  int opcion;

    do {
        printf("\n--- MENU ---\n");
        printf("1. Registrar Alumno\n");
        printf("2. Desplegar Alumnos\n");
        printf("3. Reordenar\n");
        printf("4. Buscar Alumno\n");
        printf("5. Borrar Alumno\n");
        printf("6. Salir\n");
        printf("Elige: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1: registrarAlumno(&lista); break;
            case 2: mostrarListaD(lista); break;
            case 3: reordenarLista(&lista); break;
           	case 4: buscarAlumno(&lista); break;
            case 5: borrarAlumno(&lista); break;
        }
    } while(opcion != 6);

    borrarLista(&lista);
    return 0;


/*
	ListaD lista;
	lista = inicializarListaD();
	mostrarListaD(lista);
	int x;
	char cadena[LONGITUD];
	
	inputEntero("\n Captura x: ",&x);
	inputCadena("\n Captura nombre: ",cadena,LONGITUD);
	inputEntero("\n Captura x: ",&x);
	inputCadena("\n Captura nombre: ",cadena,LONGITUD);
	inputEntero("\n Captura x: ",&x);
	inputCadena("\n Captura nombre: ",cadena,LONGITUD);

	printf("\n %d %s",x,cadena);
	printf("\n %d %s",x,cadena);

 */


}
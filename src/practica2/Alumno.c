#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Alumno.h"

void* crearAlumno(Alumno a){
    Alumno *nuevo = malloc(sizeof(Alumno));
    *nuevo = a ;
    return nuevo;
}

void imprimirAlumno(void *a){
  Alumno *aa;
  aa = a;
  printf("[Mat: %u, Nombre: %s, Semestre: %d, Prom: %.2f]\n",aa->matricula,aa->nombre,aa->semestres,aa->promedio);
}

int compararPorMatricula(void *a, void *b){
  Alumno *aa, *ab;
  aa = a;
  ab = b;
  if(aa->matricula < ab->matricula){
    return -1;
  }else if(aa->matricula > ab->matricula){
    return 1;
  }else{
    return 0;
  }
}

int compararPorNombre(void *a, void *b){
  Alumno *aa, *ab;
  aa = a;
  ab = b;
  return strcmp(aa->nombre,ab->nombre);
}

int compararPorSemestres(void *a, void *b){
  Alumno *aa, *ab;
  aa = a;
  ab = b;
  if(aa->semestres < ab->semestres){
    return -1;
  }else if(aa->semestres > ab->semestres){
    return 1;
  }else{
    return 0;
  }
}

int compararPorPromedio(void *a, void *b){
  Alumno *aa, *ab;
  aa = a;
  ab = b;
  if(aa->promedio > ab->promedio){
    return -1;
  }else if(aa->promedio < ab->promedio){
    return 1;
  }else{
    return 0;
  }
}

void registrarAlumno(ListaD *lista){
  Alumno a;
  printf("Matricula: \n");
  scanf("%u", &a.matricula);
  getchar();
  printf("Nombre: \n");
  scanf("%[^\n]", a.nombre);
  printf("Semestres: \n");
  scanf("%d", &a.semestres);
  printf("Promedio: \n");
  scanf("%f", &a.promedio);

  if(buscarDato(*lista, &a) != NULL){
    printf("Error: Matricula ya registrada.\n");
    return;
  }

  //insertarFinal(lista,crearAlumno(a));
  insertarOrdenado(lista, crearAlumno(a));

}


void buscarAlumno(ListaD *lista){

  int opcion;
  Alumno temp;

  printf("Buscar por: 1. Matricula 2. Nombre 3. Semestres 4. Promedio: ");
  scanf("%d", &opcion);

  switch(opcion){
    case 1:
      printf("Matricula: \n");
      scanf("%u", &temp.matricula);
      lista->comparar = &compararPorMatricula;
      break;
    case 2:
      printf("Nombre: \n");
      scanf(" %[^\n]", temp.nombre);
      lista->comparar = &compararPorNombre;
      break;
    case 3:
      printf("Semestres: \n");
      scanf("%d", &temp.semestres);
      lista->comparar = &compararPorSemestres;
      break;
    case 4:
      printf("Promedio: \n");
      scanf("%f", &temp.promedio);
      lista->comparar = &compararPorPromedio;
      break;
    default: printf("Opcion incorrecto.\n"); return;
  }

  Alumno *encontrado = buscarDato(*lista, &temp);
  if(encontrado != NULL ){
    imprimirAlumno(encontrado);
  }else{
    printf("Alumno no encontrado.\n");
  }
}

void borrarAlumno(ListaD *lista){
  Alumno temp;
  printf("Matricula a borrar: \n");
  scanf("%d", &temp.matricula);
  lista->comparar = &compararPorMatricula;
  borrarDato(lista, &temp);
}

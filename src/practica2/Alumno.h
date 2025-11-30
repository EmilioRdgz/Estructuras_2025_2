#ifndef ALUMNO_H
#define ALUMNO_H

#include "../Estructuras/listadoble/listadoble.h"

typedef struct {
  unsigned int matricula;
  char nombre[64];
  int semestres;
  float promedio;
}Alumno;

void* crearAlumno(Alumno a);
void imprimirAlumno(void *a);

int compararPorMatricula(void *a, void *b);
int compararPorNombre(void *a, void *b);
int compararPorSemestres(void *a, void *b);
int compararPorPromedio(void *a, void *b);

void registrarAlumno(ListaD *lista);
void reordenarLista(ListaD *lista);
void buscarAlumno(ListaD *lista);
void borrarAlumno(ListaD *lista);


#endif

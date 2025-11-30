#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../captura/captura.h"
#include "../Estructuras/hash/hashtable.h"
#include "Libro.h"
#include "../Estructuras/cola/cola.h"
#include "../Estructuras/arbol/arbol.h"

#define longitud 50

#define MAX_LIBROS_EXTRA 100

//Definir nuevos Libros
Libro librosExtra[MAX_LIBROS_EXTRA];
int contadorExtras = 0;

//Prototipos de funciones Folding
int foldingCadena(char *cadena);
int foldingISBN(void *dato);
int foldingTitulo(void *dato);
int foldingAutor(void *dato);
int foldingFecha(void *dato);
int foldingTituloInicial(void *dato);
int foldingAutorInicial(void *dato);

//Prototipos de funciones de comparar
int compararISBN(void *a, void *b);
int compararTitulo(void *a, void *b);
int compararAutor(void *a, void *b);
int compararFecha(void *a, void *b);
int compararTituloInicial(void *a, void *b);
int compararAutorInicial(void *a, void *b);

//Prototipos de Funciones para buscar dentro de la tabla Hash
void buscarISBN(HashTable *tabla);
void buscarTitulo(HashTable *tabla);
void buscarAutor(HashTable *tabla);
void buscarFecha(HashTable *tabla);

//Prototipos de funciones para busacar dentro de un rango de iniciales
void buscarRangoTitulos(HashTable *tabla);
void buscarRangoAutores(HashTable *tabla);
void buscarRangoFechas(HashTable *tabla);

//Prototipos de funciones para eliminar un libro en una tabla
int eliminarClave(HashTable *tabla, void *dato);
int buscarClaveIndice(HashTable tabla, void *dato);
void eliminarLibroDeTablas(HashTable *tablaISBN, HashTable *tablaTitulo, HashTable *tablaAutor, HashTable *tablaFecha,HashTable *tablaTituloInicial, HashTable *tablaAutorInicial );

//Prototipo de funciones para insertar Libro
void agregarLibro(HashTable *tablaISBN, HashTable *tablaTitulo, HashTable *tablaAutor, HashTable *tablaFecha, HashTable *tablaTituloInicial, HashTable *tablaAutorInicial);
void limpiarLibro(Libro *libro);

int main(void) {

    //Creación e Inicialización de las diferentes tablas hash a utilizar
    HashTable tablaISBN  = inicializarHashTable(300,imprimirLibro,compararISBN);
    HashTable tablaTitulo = inicializarHashTable(300,imprimirLibro,compararTitulo);
    HashTable tablaAutor = inicializarHashTable(300,imprimirLibro,compararAutor);
    HashTable tablaFecha = inicializarHashTable(300,imprimirLibro,compararFecha);
    HashTable tablaTituloInicial = inicializarHashTable(300, imprimirLibro,compararTituloInicial);
    HashTable tablaAutorInicial = inicializarHashTable(300, imprimirLibro,compararAutorInicial);

    //Asignar Funciones folding
    tablaISBN.folding =  foldingISBN;
    tablaTitulo.folding =  foldingTitulo;
    tablaAutor.folding =  foldingAutor;
    tablaFecha.folding =  foldingFecha;
    tablaTituloInicial.folding =  foldingTituloInicial;
    tablaAutorInicial.folding =  foldingAutorInicial;

    //Obtener libors
    Libro *libros = obtener_libros();

    for (int i = 0; i < CANT_LIBROS; i++) {
        insertarClave(&tablaISBN, &libros[i]);
        insertarClave(&tablaTitulo, &libros[i]);
        insertarClave(&tablaAutor, &libros[i]);
        insertarClave(&tablaFecha, &libros[i]);
        insertarClave(&tablaTituloInicial, &libros[i]);
        insertarClave(&tablaAutorInicial, &libros[i]);
    }

    int opcion;
    do {

        printf("\n\n[0] BUSCAR ISBN \n");
        printf("[1] BUSCAR TÍTULO EXACTO\n");
        printf("[2] BUSCAR TÍTULO POR RANGO\n");
        printf("[3] BUSCAR AUTOR EXACTO\n");
        printf("[4] BUSCAR AUTOR POR RANGO\n");
        printf("[5] BUSCAR FECHA EXACTA (AÑOS)\n");
        printf("[6] BUSCAR FECHA POR RANGO (AÑO)\n");
        printf("[7] ELIMINAR LIBRO POR ISBN\n");
        printf("[8] AGREGAR LIBRO\n");
        printf("[9] SALIR\n");
        scanf("%d", &opcion);
        clear_buffer();

        switch (opcion) {
            case 0:
                buscarISBN(&tablaISBN);
            break;
            case 1:
                buscarTitulo(&tablaTitulo);
            break;
            case 2:
                buscarRangoTitulos(&tablaTituloInicial);
            break;
            case 3:
                buscarAutor(&tablaAutor);
            break;
            case 4:
                buscarRangoAutores(&tablaAutorInicial);
            break;
            case 5:
                buscarFecha(&tablaFecha);
            break;
            case 6:
                buscarRangoFechas(&tablaFecha);
            break;
            case 7:
                eliminarLibroDeTablas(&tablaISBN, &tablaTitulo, &tablaAutor, &tablaFecha, &tablaTituloInicial, &tablaAutorInicial);
                break;
            case 8:
                agregarLibro(&tablaISBN, &tablaTitulo, &tablaAutor, &tablaFecha, &tablaTituloInicial, &tablaAutorInicial);
                break;
            case 9:
                printf("\nHASTA LUEGO!\n");
            break;
        }
    } while (opcion != 9);

}

//Función General de folding:
//A) Dividir la cadena en segmentos de 3 caracteres
//B) Sumar los valores de los 3 caracteres del segmento
//C) Multiplicar cada segmento por una ponderación fija [1,3,2,5]
//D) Sumar los valores ponderados
int foldingCadena(char *cadena) {
    int len = strlen(cadena);

    int sumaTotal = 0;
    int sumaSegmento = 0;
    int pesosPonderacion[] = {1, 3, 2, 5};
    int numPesos = sizeof(pesosPonderacion) / sizeof(pesosPonderacion[0]);
    int segmento = 0;

    for (int i = 0; i < len; i += 3) {
        sumaSegmento = 0;

        // sumar ascii en segmento de 3
        for (int j = 0; j < 3 && (i + j) < len; j++) {
            unsigned char c = cadena[i + j];
            sumaSegmento += c;
        }

        // multiplicar por peso
        sumaSegmento *= pesosPonderacion[segmento % numPesos];

        sumaTotal += sumaSegmento;
        segmento++;
    }
    return sumaTotal;
}

//Función que convierte ISBN en una cadena valida y llama a la función foldingCadena para validar
int foldingISBN(void *dato) {
    //castear dato a tipo libro
    Libro *libro = (Libro *) dato;

    char validos[50]; //crear arreglo para almacenar solo caracteres validos
    int c = 0; //indice para contar y almacenar los caracteres válidos

    //
    for (int i = 0; libro->isbn[i] != '\0'; i++) {
        if (libro->isbn[i] >= '0' && libro->isbn[i] <= '9') {
            validos[c++] = libro->isbn[i]; // guardar caracteres validos
        }
    }
    validos[c] = '\0'; //marcar cadena como terminada

    return foldingCadena(validos); //llmara función para hacer folding
}


//Función que llama a la función general folding utilizando el título del libro
int foldingTitulo(void *dato) {
    //castear dato a tipo libro
    Libro *libro = (Libro *) dato;
    return foldingCadena(libro->titulo); //llamar función general folding con el titulo del libro
}


//Función que llama a la función general folding utilizando el autor del libro
int foldingAutor(void *dato) {
    Libro *libro = (Libro *) dato;
    return foldingCadena(libro->autor);
}

//Función qe converte el año del libro a cadena y llama la función general de folding
int foldingFecha(void *dato) {
    Libro *libro = (Libro *) dato;
    char temp[10];
    sprintf(temp, "%d", libro->fecha); // convertir año a cadena

    return foldingCadena(temp);
}

//Función folding para Titulo de libro que almacen la inicial del libro y hace folding con esa letra
int foldingTituloInicial(void *dato) {

    Libro *libro = (Libro *) dato;
    char letra[2];
    letra[0] = libro->titulo[0];
    letra[1] = '\0';

    return foldingCadena(letra);
}


//Función folding para Autor por Rango, recibe un libro y almacena la 1era letra del nombre del Autor.
int foldingAutorInicial(void *dato) {
    Libro *libro = (Libro *) dato;

    char letra[2];
    letra[0] = libro->autor[0];
    letra[1] = '\0';

    return foldingCadena(letra);
}

//Función comparar ISBN
int compararISBN(void *a, void *b) {

    Libro *A = (Libro *) a;
    Libro *B = (Libro *) b;

    return strcmp((A->isbn), (B->isbn)); //regresa 0 si son iguales
}

//Función comparar por Título
int compararTitulo(void *a, void *b) {
    Libro *A = (Libro *) a;
    Libro *B = (Libro *) b;

    return strcmp((A->titulo), (B->titulo));
}

//Función comparar por Autor
int compararAutor(void *a, void *b) {
    Libro *A = (Libro *) a;
    Libro *B = (Libro *) b;

    return strcmp(A->autor, B->autor);
}

//Función comparar por Fecha
int compararFecha(void *a, void *b) {
    Libro *A = (Libro *) a;
    Libro *B = (Libro *) b;

    return A->fecha - B->fecha;
}

//Función comparar por Inicial de Titulo
int compararTituloInicial(void *a, void *b) {
    Libro *A = (Libro *) a;
    Libro *B = (Libro *) b;

    return (A->titulo[0] - B->titulo[0]);
}


//Función comparar por Autor Inicial
int compararAutorInicial(void *a, void *b) {
    Libro *A = (Libro *) a;
    Libro *B = (Libro *) b;

    return (A->autor[0] - B->autor[0]);
}

//Funcione Para buscar libro por ISBN exacto
void buscarISBN(HashTable *tabla) {

    char isbnBuscado[50];
    inputCadena("\nIngresa el ISBN a buscar: ", isbnBuscado,longitud);

    Libro temp;
    strcpy(temp.isbn, isbnBuscado);

    Cola resultado = buscarClave(tabla, &temp);

    if (vaciaC(resultado)) {
        printf("\nNo se econtró nigún libro con ese ISBN \n");
        return;
    }

    Arbol arbol = inicializarArbol(imprimirLibro, compararISBN);
     while (!vaciaC(resultado)) {
         Libro *lib = dequeue(&resultado);
         insertarArbol(&arbol, lib);
     }
    printf("ISBN encontrado!\n");
    imprimirOrden(arbol, ORDEN);
}

//Función para buscar Libro por Título exacto
void buscarTitulo(HashTable *tabla) {
    char tituloBuscado[50];
    inputCadena("\nIngresa el título del libro a buscar: ", tituloBuscado,longitud);

    Libro temp;
    strcpy(temp.titulo, tituloBuscado);

    Cola resultado = buscarClave(tabla, &temp);

    if (vaciaC(resultado)) {
        printf("\nNo se encontró ningún libro con el título ingresado\n");
        return;
    }

    Arbol arbol = inicializarArbol(imprimirLibro, compararTitulo);

    while (!vaciaC(resultado)) {
        Libro *lib = dequeue(&resultado);
        insertarArbol(&arbol, lib);
    }

    printf("Título encontrado!\n");
    imprimirOrden(arbol, ORDEN);
}


//Función para buscar Libro por Autor (Nombre exacto del Autor)
void buscarAutor(HashTable *tabla) {
    char autorBuscado[50];
    inputCadena("\nIngresa el nombre del Autor a buscar: ", autorBuscado,longitud);

    Libro temp;
    strcpy(temp.autor, autorBuscado);

    Cola resultado = buscarClave(tabla, &temp);

    if (vaciaC(resultado)) {
        printf("\nNo se encontró nigún Libro con el Autor ingresado\n");
        return;
    }
    Arbol arbol = inicializarArbol(imprimirLibro, compararAutor);

    while (!vaciaC(resultado)) {
        Libro *lib = dequeue(&resultado);
        insertarArbol(&arbol, lib);
    }

    printf("Autor encontrado!\n");
    imprimirOrden(arbol, ORDEN);
}

//Función para buscar Libro por Fecha (Año)
void buscarFecha(HashTable *tabla) {
    int fecha;
    inputEntero("\nIngresa el año del libro o libros a buscar: ", &fecha);

    Libro temp;
    temp.fecha = fecha;

    Cola resultado = buscarClave(tabla, &temp);

    if (vaciaC(resultado)) {
        printf("\nNo se encontró nigún Libro publicado en ese año\n");
        return;
    }

    Arbol arbol = inicializarArbol(imprimirLibro, compararFecha);

    while (!vaciaC(resultado)) {
        Libro *lib = dequeue(&resultado);
        insertarArbol(&arbol, lib);
    }

    printf("Fecha encontrada!\n");
    imprimirOrden(arbol, ORDEN);
}

//Función para buscar Título por Rango
void buscarRangoTitulos(HashTable *tabla) {

    char inicio, fin;

    printf("\nBuscar Títulos por Rango de Iniciales\n");
    inputChar("\nIngresa la letra inicial del rango a buscar:", &inicio);
    inputChar("\nIngresa la letra final del rango a buscar: ",&fin);

    //Convertir a mayuscula
    inicio = toupper(inicio);
    fin = toupper(fin);
    //validar que rango sea válido
    if (inicio > fin) {
        printf("\nRango Inválido\n");
        return;
    }

    //Arbol ordenado por Título completo
    Arbol arbol = inicializarArbol(imprimirLibro, compararTitulo);

    //Recorrer desde la letra inicio hasta letra fin
    for (char letra = inicio; letra <= fin; letra++)
    {
        for (int i = 0; i < tabla->tam; i++) //recorrer la tabla
        {
            if (tabla->tabla[i] != NULL && tabla->tabla[i] != (void*)-1) // ignorar espacios vacíos o marcados como eliminados (-1)
            {
                Libro *lib = (Libro*) tabla->tabla[i];
                //si inicial es igual entonces insertar en arbol
                if (toupper(lib->titulo[0]) == letra)
                    insertarArbol(&arbol, lib);
            }
        }
    }
    //imprimir libros encontrados en el rango
    printf("\nLibros encontrados en el Rango '%c'-'%c': \n",inicio, fin);
    imprimirOrden(arbol, ORDEN);
}

//Función para buscar Autores y sus libros en un rango de iniciales
void buscarRangoAutores(HashTable *tabla) {

    char inicio, fin;
    printf("\nBuscar Autores por Rango de Iniciales\n");
    inputChar("\nLetra inicial del rango a buscar: ", &inicio);
    inputChar("\nLetra final del rango buscar: ",&fin);

    inicio = toupper(inicio);
    fin = toupper(fin);
    //validar rangos
    if (inicio > fin) {
        printf("\nRango Inválido\n");
        return;
    }

    //Arbol ordenado por Autor Completo
    Arbol arbol = inicializarArbol(imprimirLibro, compararAutor);
    //recorrer desde la letra inicio a la letra fin (rango ingresado por usuario)
    for (char letra = inicio; letra <= fin; letra++)
    {
        for (int i = 0; i < tabla->tam; i++) //recorrrer toda la tabla
        {
            if (tabla->tabla[i] != NULL && tabla->tabla[i] != (void*)-1) //ignorar esapcios vacíos o marcados como eliminados (-1)
            {
                Libro *lib = (Libro*) tabla->tabla[i];
                //si inicial es igual entonces insertar en el árbol
                if (toupper(lib->autor[0]) == letra)
                    insertarArbol(&arbol, lib);
            }
        }
    }
    //imprimir libros encontrados por iniciales de autores
    printf("\nLibros encontrados por iniciales '%c'-'%c' de Autores: \n", inicio, fin);
    imprimirOrden(arbol, ORDEN);
}

//Función para buscar un Libro por un rango determinado de años
void buscarRangoFechas(HashTable *tabla) {
    int inicio, fin;

    printf("\nBuscar Libro por Rango de años\n");
    inputEntero("\nIngresa el año inicial del rango a buscar: ", &inicio);
    inputEntero("\nIngres el año final del rango a buscar: ",&fin);
    //validar rango
    if (inicio > fin) {
        printf("\nRango Inválido\n");
        return;
    }
    //Inicializar arbo y asignar funciones
    Arbol arbol = inicializarArbol(imprimirLibro, compararFecha);
    //recorrer TODA la tabla de Fechas
    for (int i = 0; i < tabla->tam; i++) {
        if (tabla->tabla[i] != NULL && tabla->tabla[i] != (void*)-1) {
            Libro *lib = (Libro*) tabla->tabla[i];
            if (lib->fecha >= inicio && lib->fecha <= fin) {
                insertarArbol(&arbol, lib);
            }
        }
    }
    //imprimir libros en el rango de años
    printf("\nLibros publicados entre %d - %d \n", inicio, fin);
    imprimirOrden(arbol, ORDEN);
}

//Función que busca y regresa el indice de un libro en una tabla, -1 si no existe el indice
int buscarClaveIndice(HashTable tabla, void *dato) {
    //generar clave (folding) y calcular el indice usando hash
    int clave = tabla.folding(dato);
    int indice = hash(clave, tabla.tam);
    // si vacía entonce dato no esta en la tabla
    if (tabla.tabla[indice]  == NULL) {
        return -1;
    }


    int k = 0, nuevo_indice;

    //recorrer posiciones usando rehashing
    do {
        nuevo_indice = rehash(indice, k, tabla.tam);
        //si nulo entonce no esta el dato
        if (tabla.tabla[nuevo_indice] == NULL) {
            return -1;
        }
        // si casilla no esta marcada como que borrada
        if (tabla.tabla[nuevo_indice] != (void*)-1) {
            //comparar si esta el dato exacto
            if (tabla.comparar(tabla.tabla[nuevo_indice], dato) ==  0) {
                return nuevo_indice;
            }
        }
        k++;
    }while(tabla.tabla[nuevo_indice] != NULL); //mientras no haya indice en null
    return -1; // si no se encontro el dato
}

//Función principal que elimina el dato de las tablas y reacomoda los elementos siguientes
int eliminarClave(HashTable *tabla, void *dato)
{
    //generar clave (folding) y obtener indice con hash.
    int clave = tabla->folding(dato);
    int indice = hash(clave, tabla->tam);
    // si vacia entonces no esta en la tabla
    if (tabla->tabla[indice] == NULL)
        return 0;

    int k = 0;
    int pos;
    //buscar dato con rehashing
    do {
        pos = rehash(indice, k, tabla->tam); //calcular siguiente posicion

        if (tabla->tabla[pos] == NULL) //si null entonces no esra
            return 0;

        if (tabla->comparar(tabla->tabla[pos], dato) == 0) //si función comparar regresa 0 entonces encontramos el dato
            break;

        k++;
    } while (1);

    //Eliminar elemento encontrado y decrementar cantidad
    tabla->tabla[pos] = NULL;
    tabla->cant--;

    //Reinsertar los que venia despues
    k++;
    int siguiente;

    while (1) {
        //siguiente dato afectado
        siguiente = rehash(indice, k, tabla->tam);
        // si esta nulo entonces no hay más elementos a mover
        if (tabla->tabla[siguiente] == NULL)
            break;
        //guardar elemento desacomodado
        void *temp = tabla->tabla[siguiente];
        //borrar temporalmente de la tabla
        tabla->tabla[siguiente] = NULL;
        tabla->cant--;
        //reinsertarlo
        insertarClave(tabla, temp);

        k++;
    }

    return 1; // si elimino existosamente
}

//Funcion que elimina un libro de todas las tablas hash
void eliminarLibroDeTablas(HashTable *tablaISBN, HashTable *tablaTitulo, HashTable *tablaAutor, HashTable *tablaFecha,HashTable *tablaTituloInicial, HashTable *tablaAutorInicial ) {
    char isbn[50];
    inputCadena("\nIngresa el ISBN del Libro a eliminar: ",isbn, 50);
    //crear copia temporal del libro
    Libro temp;
    strcpy(temp.isbn,isbn);
    //Buscar libro usando la tablaISBN
    Cola resultado = buscarClave(tablaISBN, &temp);
    //si cola vacia entonces no existe un libro con ese ISBN
    if (vaciaC(resultado)) {
        printf("\nNo existe un libro con ese ISBN\n");
        return;
    }
    //dequeue a los libros encontreados con ese ISBN
    Libro *lib = dequeue(&resultado);
    //printf("\nLibro encontrado");
    imprimirLibro(lib);
    //Eliminar libro de la tabla llamando a la función principal
    eliminarClave(tablaISBN, lib);
    eliminarClave(tablaTitulo, lib);
    eliminarClave(tablaAutor, lib);
    //eliminarClave(tablaFecha, lib);
    //Eliminar de tablaFecha
    //Crea copia del libro apuntando a la fecha exacta y elimina ese libro
    //Libro LFecha = {0};
    //LFecha.fecha = lib->fecha;
    //eliminarClave(tablaFecha, &LFecha);

    //Eliminar de tablaFecha buscando el año exacto
    for (int i = 0; i < tablaFecha->tam; i++)
    {
        if (tablaFecha->tabla[i] != NULL && tablaFecha->tabla[i] != (void*)-1)
        {
            Libro *aux = (Libro*) tablaFecha->tabla[i];

            if (aux->fecha == lib->fecha && aux == lib)  // si es el mismo año entonces marcar como eliminado y reducir cant
            {
                tablaFecha->tabla[i] = (void*) -1;
                tablaFecha->cant--;
                break;
            }
        }
    }



    //Eliminar de tablaTituloInicial
    //Utiliza la incial del libro como clave y hace eliminación manual recorriendo toda la tabla
    for (int i = 0; i < tablaTituloInicial->tam; i++)
    {
        //ignorar espacios(indices) vacios y o eliminados
        if (tablaTituloInicial->tabla[i] != NULL && tablaTituloInicial->tabla[i] != (void*)-1)
        {
            Libro *aux = (Libro*) tablaTituloInicial->tabla[i];

            // Comparar título completo
            if (strcmp(aux->titulo, lib->titulo) == 0)
            {
                //marcar espcaio/indice como eliminado y reducir cantidad
                tablaTituloInicial->tabla[i] = (void*) -1;
                tablaTituloInicial->cant--;
                break;
            }
        }
    }

    //Eliminar libro de tablaAutorInicial utilizando la letra inicial para buscar el indice y recorrer la tabla
    for (int i = 0; i < tablaAutorInicial->tam; i++)
    {
        //ignorar vacios
        if (tablaAutorInicial->tabla[i] != NULL && tablaAutorInicial->tabla[i] != (void*)-1)
        {
            Libro *aux = (Libro*) tablaAutorInicial->tabla[i];
            //comparar autor completo
            if (strcmp(aux->autor, lib->autor) == 0)
            {
                //marcar espacio/indice como borrado y reducir cantidad.
                tablaAutorInicial->tabla[i] = (void*) -1;
                tablaAutorInicial->cant--;
                break;
            }
        }
    }

}

void agregarLibro(HashTable *tablaISBN, HashTable *tablaTitulo, HashTable *tablaAutor, HashTable *tablaFecha, HashTable *tablaTituloInicial, HashTable *tablaAutorInicial)
{
    if (contadorExtras >= MAX_LIBROS_EXTRA) {
        printf("\nNo se pueden agregar más libros.\n");
        return;
    }


    //Crear libro nuevo
    Libro *nuevo = &librosExtra[contadorExtras];

    limpiarLibro(nuevo);

    //Datos del nuevo libro
    inputCadena("ISBN: ", nuevo->isbn, 50);
    inputCadena("Título: ", nuevo->titulo, 80);
    inputCadena("Autor: ", nuevo->autor, 80);
    inputEntero("Año: ", &nuevo->fecha);

    printf("Estado (1=DISPONIBLE, 0=PRESTADO): ");
    scanf("%d", &nuevo->disponible);

    // Insertar en TODAS las tablas
    insertarClave(tablaISBN, nuevo);
    insertarClave(tablaTitulo, nuevo);
    insertarClave(tablaAutor, nuevo);
    insertarClave(tablaFecha, nuevo);
    insertarClave(tablaTituloInicial, nuevo);
    insertarClave(tablaAutorInicial, nuevo);

    contadorExtras++;

    printf("\nLibro agregado\n");
}

void limpiarLibro(Libro *libro) {
    int i;

    // Limpiar ISBN
    for (i = 0; i < 50; i++)
        libro->isbn[i] = '\0';

    // Limpiar Título
    for (i = 0; i < 80; i++)
        libro->titulo[i] = '\0';

    // Limpiar Autor
    for (i = 0; i < 80; i++)
        libro->autor[i] = '\0';

    // Limpiar fecha
    libro->fecha = 0;

    // Limpiar disponibilidad
    libro->disponible = 0;
}

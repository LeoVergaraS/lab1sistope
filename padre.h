#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct game{
    int id;
    char nombre[3000];
    int restriccion;
    float precio;
    int proximamente;
    int fecha;
    int gratis;
    int win;
    int mac;
    int lin;
} juego;

juego *nuevoJuego();
int transformarStringABool(char *cadena);
juego *listaDeJuegos(char *nombreArchivo, int *i);
int* escribirJuego(char *nombreSalida, juego *juego, int i,int *n, float precioMinimo);
void merge(juego* juegos,int inicio,int medio,int fin);
void mergeSort(juego* juegos, int ini, int fin);
int* agregarCabecera(int* anios, int anio, int *n);
int* arrayCabeceras(juego* lista, char *nombreArchivo, int *i);
void escribirArchivoFinal(char *string,char* nombreSalida);
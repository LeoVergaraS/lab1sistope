#include <stdio.h>

juego *listaDeJuegosHijo(int comienzo, int fin, int *i);
void calculosDelHijo(juego* juegos, int n, int *c, int *b, float *prom);
int* agregarElemento(int* arreglo,int a,int *n);
int* listaJuegosGratis(juego* juegos, int tam,int *n);
void promedioPorPlataforma(juego* juegos, int n, int *w, int *m, int *l);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "juego.h"

juego *listaDeJuegosHijo(int comienzo, int fin, int *i);
void calculosDelHijo(juego* juegos, int n, int *c, int *b, float *prom, float precioMinimo, int contador);
void promedioPorPlataforma(juego* juegos, int n, float *w, float *m, float *l, float precioMinimo, int contador);
int contadorJuegosPrecioMinimo(juego* juegos, int n, int precioMinimo);
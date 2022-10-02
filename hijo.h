#include <stdio.h>

juego *listaDeJuegosHijo(int comienzo, int *i);
void calculosDelHijo(juego* juegos, int n, int *k, int *l, int *prom);
juego* agregarJuego(juego* juegos, juego juego, int *n);
juego* juegosGratis(juego* juegos, int *n);
void promedioPorPlataforma(juego* juegos, int n, float *w, float *m, float *l);
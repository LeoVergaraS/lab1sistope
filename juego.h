#ifndef JUEGO_H
#define JUEGO_H

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

#endif
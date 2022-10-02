#include <stdio.h>
#include "hijo.h"

juego *listaDeJuegosHijo(int comienzo, int *i)
{
    FILE *archivo = fopen("intermedio.txt", "r");
    fseek(archivo,comienzo,SEEK_SET);
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo");
        return NULL;
    }
    char linea[255];
    //Se cuentan las lineas del archivo
    while (fgets(linea, 255, archivo) != NULL)
    {
        *i = *i + 1;
    }
    juego *lista = (juego *)malloc(sizeof(juego) * (*i));
    char prox[255], gratis[255], lin[255], mac[255], win[255];
    fseek(archivo,comienzo,SEEK_SET);
    int j = 0;
    int resto;
    int p;
    char *token;
    char copia[255];
    //Se lee el archivo y se guarda en el arreglo de estructuras
    while (j < *i)
    {
        int contador = 0;
        fgets(linea, 255, archivo);
        for (int k = 0; k < strlen(linea); k++)
        {
            if (linea[k] == ',')
            {
                contador++;
            }
        }
        if (contador == 9)
        {
            sscanf(linea, "%ld,%[^,],%d,%f,%[^,],%d,%[^,],%[^,],%[^,],%[^,]", &lista[j].id, lista[j].nombre, &lista[j].restriccion, &lista[j].precio, prox, &lista[j].fecha, gratis, win, mac, lin);
            lista[j].proximamente = transformarStringABool(prox);
            lista[j].gratis = transformarStringABool(gratis);
            lista[j].win = transformarStringABool(win);
            lista[j].mac = transformarStringABool(mac);
            lista[j].lin = transformarStringABool(lin);
        }
        //Se guarda en el caso de que el juego tenga una coma en el nombre
        else
        {
            p = 0;
            resto = contador - 9;
            token = strtok(linea, ",");
            while (p<9)
            {
                if (p == 0)
                {
                    sscanf(token, "%ld", &lista[j].id);
                }
                else if (p == 1)
                {
                    strcpy(lista[j].nombre, token);
                    while (resto > 0)
                    {
                        token = strtok(NULL, ",");
                        strcpy(copia, token);
                        strcat(lista[j].nombre, copia);           
                        resto--;
                    }
                }
                else if (p == 2)
                {
                    sscanf(token, "%d", &lista[j].restriccion);
                }
                else if (p == 3)
                {
                    sscanf(token, "%f", &lista[j].precio);
                }
                else if (p == 4)
                {
                    strcpy(prox, token);
                    lista[j].proximamente = transformarStringABool(prox);
                }
                else if (p == 5)
                {
                    sscanf(token, "%d", &lista[j].fecha);
                }
                else if (p == 6)
                {
                    strcpy(gratis, token);
                    lista[j].gratis = transformarStringABool(gratis);
                }
                else if (p == 7)
                {
                    strcpy(win, token);
                    lista[j].win = transformarStringABool(win);
                }
                else if (p == 8)
                {
                    strcpy(mac, token);
                    lista[j].mac = transformarStringABool(mac);
                }
                else if (p == 9)
                {
                    strcpy(lin, token);
                    lista[j].lin = transformarStringABool(lin);
                }
                p++;
                token = strtok(NULL, ",");
            }
        }
        j++;
    }
    fclose(archivo);
    return lista;
}

void calculosDelHijo(juego* juegos, int n, int *k, int *l, float *prom){
    int caro = juegos[0].precio;
    int barato = juegos[0].precio;
    int suma = 0;
    for(int i=0 ;i<n;i++){
        if(juegos[i].precio > caro){
            caro = juegos[i].precio;
            *k = i;
        }
        else if(juegos[i].precio < barato){
            barato = juegos[i].precio;
            *l = i;
        }
        suma = suma + juegos[i].precio;
    }
    *prom = suma/n;
}

juego* agregarJuego(juego* juegos, juego juego, int *n){
    int i;
    juego* nuevoJuegos = (juego*)malloc(sizeof(juego)*(*n+1));
    for(i=0;i<*n;i++){
        nuevoJuegos[i] = juegos[i];
    }
    nuevoJuegos[*n] = juego;
    *n = *n + 1;
    free(juegos);
    return nuevoJuegos;
}

juego* juegosGratis(juego* juegos, int *n){
    int i;
    juego* juegosGratis = (juego*)malloc(sizeof(juego)*(*n));
    for(i=0;i<n;i++){
        if(juegos[i].gratis == 1){
            juegosGratis = agregarJuego(juegosGratis, juegos[i], &(*n));
        }
    }
    return juegosGratis;
}

void promedioPorPlataforma(juego* juegos, int n, float *w, float *m, float *l){
    int win = 0;
    int mac = 0;
    int lin = 0;
    for(int i=0 ;i<n;i++){
        if(juegos[i].win == 1){
            win++;
        }
        if(juegos[i].mac == 1){
            mac++;
        }
        if(juegos[i].lin == 1){
            lin++;
        }
    }
    *w = win/n;
    *m = mac/n;
    *l = lin/n;
}



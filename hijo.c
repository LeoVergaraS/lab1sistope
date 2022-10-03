#include <stdio.h>
#include "hijo.h"

//Lee el archivo intermedio y crea una lista de juegos por cada hijo
//Entrada: comienzo y fin del año
//Salida: lista de juegos
juego *listaDeJuegosHijo(int comienzo, int fin, int *i)
{
    FILE *archivo = fopen("intermedio.txt", "r");
    fseek(archivo,comienzo,SEEK_SET);
    int pos = ftell(archivo);
    if (archivo == NULL){
        printf("No se pudo abrir el archivo");
        return NULL;
    }
    char linea[2000];
    //Se cuentan las lineas del archivo
    while (fgets(linea, 2000, archivo) != NULL)
    {
        *i = *i + 1;
        pos = ftell(archivo);
        if(pos == fin){
            break;
        }
        
    }
    juego *lista = (juego *)malloc(sizeof(juego) * (*i));
    fseek(archivo,comienzo,SEEK_SET);
    pos = ftell(archivo);
    //Se lee el archivo y se guarda en el arreglo de estructuras
    int j = 0;
    fgets(linea, 2000, archivo);
    ////////////////////////////////////////////////////////////////////// //printf("%s",linea);
    fseek(archivo,comienzo,SEEK_SET);
    while (j < *i)
    {
        fgets(linea, 2000, archivo);
        //tokenize linea

        char *token = strtok(linea, ";");
        lista[j].id = atoi(token);
        token = strtok(NULL, ";");
        strcpy(lista[j].nombre, token);
        token = strtok(NULL, ";");
        lista[j].restriccion = atoi(token);
        token = strtok(NULL, ";");
        lista[j].precio = atof(token);
        token = strtok(NULL, ";");
        lista[j].proximamente = atoi(token);
        token = strtok(NULL, ";");
        lista[j].fecha = atoi(token);
        token = strtok(NULL, ";");
        lista[j].gratis = atoi(token);
        token = strtok(NULL, ";");
        lista[j].win = atoi(token);
        token = strtok(NULL, ";");
        lista[j].mac = atoi(token);
        token = strtok(NULL, ";");
        lista[j].lin = atoi(token);
        j++;
    }
    //printf("aqui: %d  %s  %d  %f  %d  %d  %d  %d  %d  %d\n", lista[0].id, lista[0].nombre, lista[0].restriccion, lista[0].precio, lista[0].proximamente, lista[0].fecha, lista[0].gratis, lista[0].win, lista[0].mac, lista[0].lin);
    pos = ftell(archivo);
    fclose(archivo);
    return lista;
}

//Calcula los promedios, el más caro y el más barato de los juegos por cada hijo
//Entrada: lista de juegos, tamaño de la lista, punteros a los promedios, el más caro y el más barato
//Salida: nada
void calculosDelHijo(juego* juegos, int n, int *c, int *b, float *prom){
    int caro = juegos[0].precio;
    int barato = juegos[0].precio;
    float suma = 0;
    for(int i=0 ;i<n;i++){
        if(juegos[i].precio > caro){
            caro = juegos[i].precio;
            *c = i;
        }
        else if(juegos[i].precio < barato){
            barato = juegos[i].precio;
            *b = i;
        }
        suma = suma + juegos[i].precio;
    }
    *prom = suma/n;
    //printf("caro: %d, barato: %d, promedio: %f\n",caro,barato,*prom);
    //sprintf(stringFinal, "%d;Año:%d\nJuego mas caro:%s\nJuego mas barato:%s\nPromedio de precios:%f\n",fecha,fecha,juegos[k].nombre,juegos[l].nombre,prom);

    return ;

}

//Calcula la cobertura de cada plataforma por año
//Entrada: lista de juegos, tamaño de la lista, punteros a los promedios, el más caro y el más barato
//Salida: nada
void promedioPorPlataforma(juego* juegos, int n, int *w, int *m, int *l){
    //printf("%d %d\n",juegos[1].fecha,juegos[1].win);
    int win = 0;
    int mac = 0;
    int lin = 0;
    char* stringFinal;
    for(int i=0 ;i<n;i++){
        //printf("%d %d\n",juegos[i].fecha,juegos[i].win);
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
    *w = (win/n)*100;
    *m = (mac/n)*100;
    *l = (lin/n)*100;
    //sprintf(stringFinal, "Windows: %f%% Mac: %f%% Linux: %f%%\n", w, m, l);
    return;
}



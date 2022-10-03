#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "padre.h"

// Funcion que crea una lista de juegos
// Entrada: void
// Salida: lista de juego
juego *nuevoJuego()
{
    juego *lista = (juego *)malloc(sizeof(juego));
    return lista;
}

// Función que transforma un string a un entero 0 o 1
// Entrada: string
// Salida: entero
int transformarStringABool(char *cadena)
{
    // Si 
    if (strcmp(cadena, "Yes") == 0 || strcmp(cadena, "True") == 0)
    {
        return 1;
    }
    else 
    {
        return 0;
    }

}

// Función que lee el archivo y lo guarda en un arreglo de estructuras
// Entrada: Nombre del archivo
// Salida: Arreglo de estructuras juegos
juego *listaDeJuegos(char *nombreArchivo, int *i)
{
    FILE *archivo = fopen(nombreArchivo, "r");
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
    rewind(archivo);
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
            sscanf(linea, "%d,%[^,],%d,%f,%[^,],%d,%[^,],%[^,],%[^,],%[^,]", &lista[j].id, lista[j].nombre, &lista[j].restriccion, &lista[j].precio, prox, &lista[j].fecha, gratis, win, mac, lin);
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
                    sscanf(token, "%d", &lista[j].id);
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


int* agregarCabecera(int* cabeceras, int cabecera, int *n){
    int i;
    int* nuevasCabeceras = (int*)malloc(sizeof(int)*(*n+1));
    for(i=0;i<*n;i++){
        nuevasCabeceras[i] = cabeceras[i];
    }
    nuevasCabeceras[*n] = cabecera;
    *n = *n + 1;
    free(cabeceras);
    return nuevasCabeceras;
}

// Función que escribe el juego
// Entrada: Estructura juego
// Salida: void
int* escribirJuego(char *nombreSalida, juego *juego, int i,int *n)
{
    int * cabeceras= (int*)malloc(sizeof(int)*(*n));
    int len;
    FILE *archivo= fopen(nombreSalida, "w+");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo");
        return NULL;
    }
    int fechaActual = juego[0].fecha;
    for (int j = 0; j < i-1; j++)
    {
        fprintf(archivo, "%d;%s;%d;%f;%d;%d;%d;%d;%d;%d\n", juego[j].id, juego[j].nombre, juego[j].restriccion, juego[j].precio, juego[j].proximamente, juego[j].fecha, juego[j].gratis, juego[j].win, juego[j].mac, juego[j].lin);
        len = ftell(archivo);
        
        if(juego[j+1].fecha != fechaActual){
            cabeceras = agregarCabecera(cabeceras, len, &(*n));
            fechaActual = juego[j+1].fecha;
            if(j+1 == i-1){
                fprintf(archivo, "%d;%s;%d;%f;%d;%d;%d;%d;%d;%d\n", juego[j+1].id, juego[j+1].nombre, juego[j+1].restriccion, juego[j+1].precio, juego[j+1].proximamente, juego[j+1].fecha, juego[j+1].gratis, juego[j+1].win, juego[j+1].mac, juego[j+1].lin);
                len = ftell(archivo);
                cabeceras = agregarCabecera(cabeceras, len, &(*n));
                break;
            }
        }
    }
        
    fclose(archivo);
    return cabeceras;
}


//Esta funcion es el mergesort para ordenar por anios
//Parametros: arreglo de enteros, arreglo de enteros, entero, entero
//Retorno: void, se ordena por dentro
void merge(juego* juegos,int inicio,int medio,int fin){
	// Se inicia en cero una lista de struct de datos temporal
	// para dejar los datos.
	juego* temp = (juego*)calloc(fin-inicio+1,sizeof(juego));
	// Se establecen el inicio del arreglo izquierdo y el derecho.
	int izq = inicio;
	int der = medio+1;
	int k = 0;

	// Se sigue copiando hasta que se llegue al final de 
	// alguna de las 2 listas.
	while(izq<=medio && der<=fin){
		if(juegos[izq].fecha<=juegos[der].fecha){
			// Si la cantidad que se repite del elemento izquierdo
			// es el menor se copia en la lista temporal.
			temp[k] = juegos[izq];
			izq++;
		}else{
			// Si no, se copia el elemento derecho en la lista temporal. 
			temp[k] = juegos[der];
			der++;
		}
		k++;
	} 
	// Cuando uno de los 2 se pase del final, se tiene 
	// que copiar el arreglo que sobre.

	// Se supone que en el arreglo izquierdo quedaron datos.
	for(;izq<=medio;izq++){
		temp[k] = juegos[izq];
		k++;
	}

	// Se supone que en el arreglo derecho quedaron datos.
	for(;der<=fin;der++){
		temp[k] = juegos[der];
		k++;
	}

	// Se copia la lista temporal en el arreglo de datos.
	int l=0;
	for(k=inicio;k<=fin;k++){
		juegos[k] = temp[l];
		l++;
	}
	// Se libera el espacio de la lista temporal.
	free(temp);
	return;
}


void mergeSort(juego* juegos, int ini, int fin){
	// Caso base, se tiene solo 1 elemento.
	if(ini==fin){
		return;
	} else{ // Se tiene mas de 1 elemento.
		// Se calcula el medio del arreglo.
		int med = (ini+fin)/2;
		// Se ordena el arreglo, pero desde el inicio hasta el medio.
		mergeSort(juegos,ini,med);
		// Se ordena el arreglo, pero desde el medio+1 hasta el fin.
		mergeSort(juegos,med+1,fin);
		// Se unen la lista ordenadamente.
		merge(juegos,ini,med,fin);
		return;
	}
}



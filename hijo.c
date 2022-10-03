#include <stdio.h>
#include "hijo.h"

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
//k: caro
//l: barato
//prom: promedio
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

int* agregarElemento(int* Lista, int elemento, int *n){
    int i;
    int* nuevasLista = (int*)malloc(sizeof(int)*(*n+1));
    for(i=0;i<*n;i++){
        nuevasLista[i] = Lista[i];
    }
    nuevasLista[*n] = elemento;
    *n = *n + 1;
    free(Lista);
    return nuevasLista;
}

/* int* listaJuegosGratis(juego* juegos, int tam,int *n,int* gratis){
    int i;
    int* juegosGratis = (int*)malloc(sizeof())
    for(i=0;i<tam;i++){
        //printf("juegos[i].gratis: %d\n",juegos[i].gratis);
        if(juegos[i].gratis == 1){
            //printf("juegos[i].gratis: %d\n",juegos[i].id);
           gratis = agregarElemento(gratis,i,&(*n));
           //printf("juegos[i].gratis: %d\n",juegos[i].gratis);
           //printf("aaajuegosGratis: %d\n",juegosGratis[*n-1]);
        }
    }
    for(int j=0;j<*n;j++){
		printf( "%d\n",gratis[j]);
	}
    return gratis;
} */

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

void merge2(int* numeros,int inicio,int medio,int fin){
	// Se inicia en cero una lista de struct de datos temporal
	// para dejar los datos.
	int* temp = (int*)calloc(fin-inicio+1,sizeof(int));
	// Se establecen el inicio del arreglo izquierdo y el derecho.
	int izq = inicio;
	int der = medio+1;
	int k = 0;

	// Se sigue copiando hasta que se llegue al final de 
	// alguna de las 2 listas.
	while(izq<=medio && der<=fin){
		if(numeros[izq]<=numeros[der]){
			// Si la cantidad que se repite del elemento izquierdo
			// es el menor se copia en la lista temporal.
			temp[k] = numeros[izq];
			izq++;
		}else{
			// Si no, se copia el elemento derecho en la lista temporal. 
			temp[k] = numeros[der];
			der++;
		}
		k++;
	} 
	// Cuando uno de los 2 se pase del final, se tiene 
	// que copiar el arreglo que sobre.

	// Se supone que en el arreglo izquierdo quedaron datos.
	for(;izq<=medio;izq++){
		temp[k] = numeros[izq];
		k++;
	}

	// Se supone que en el arreglo derecho quedaron datos.
	for(;der<=fin;der++){
		temp[k] = numeros[der];
		k++;
	}

	// Se copia la lista temporal en el arreglo de datos.
	int l=0;
	for(k=inicio;k<=fin;k++){
		numeros[k] = temp[l];
		l++;
	}
	// Se libera el espacio de la lista temporal.
	free(temp);
	return;
}


void mergeSort2(int* numeros, int ini, int fin){
	// Caso base, se tiene solo 1 elemento.
	if(ini==fin){
		return;
	} else{ // Se tiene mas de 1 elemento.
		// Se calcula el medio del arreglo.
		int med = (ini+fin)/2;
		// Se ordena el arreglo, pero desde el inicio hasta el medio.
		mergeSort2(numeros,ini,med);
		// Se ordena el arreglo, pero desde el medio+1 hasta el fin.
		mergeSort2(numeros,med+1,fin);
		// Se unen la lista ordenadamente.
		merge2(numeros,ini,med,fin);
		return;
	}
}


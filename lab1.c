#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "padre.c"
#include "hijo.c"
#include "otros.c"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]){
	int i;
	int option;
	char nombreEntrada[255];
	char nombreSalida[255];
	int anioInicio;
	float precioMinimo=0;
	int bandera = 0;

	while ((option = getopt(argc, argv, ":i:o:d:p:b")) != -1){
		switch (option){
		case 'i':
			strcpy(nombreEntrada, optarg);
			break;
		case 'o':
			strcpy(nombreSalida, optarg);
			break;
		case 'd':
			sscanf(optarg, "%d", &anioInicio);
			break;
		case 'p':
			sscanf(optarg, "%f", &precioMinimo);
			break;
		case 'b':
			bandera = 1;
			break;
		case '?':
			printf("unknown option: %c\n", optopt);
			break;
		case ':':
			printf("Missing arg for %c\n", optopt);
			break;
		}
	}

	// Se lee el archivo de entrada.
	int cantidadJuegos = 0;
	juego *listaJuegos = listaDeJuegos(nombreEntrada, &cantidadJuegos);

	// Se ordena con mergeSort
	mergeSort(listaJuegos, 0, cantidadJuegos - 1);

	// Se crea el archivo intermedio.
	int cantidadAnios = 0;
	char *archivoIntermedio = "intermedio.txt";
	int *cabeceras = escribirJuego(archivoIntermedio, listaJuegos, cantidadJuegos, &cantidadAnios, precioMinimo);

	// Se genera un arreglo para almacenar los pids de cada proceso generado
	int pids[cantidadAnios];
	// Se definen un pipes para la comunicacion entre proceso padre y sus hijos
	int pipesPH[cantidadAnios][2]; // Comunicacion Padre ---> Hijo
	int pipesHP[cantidadAnios][2]; // Comunicacion Hijo ---> Padre

	// Se generan los pipe de Padre Hijo
	for (i = 0; i < cantidadAnios; i++){
		if (pipe(pipesPH[i]) == -1){
			// ERROR
			return 0;
		}
	}
	// Se generan los pipe de Hijo Padre
	for (i = 0; i < cantidadAnios; i++){
		if (pipe(pipesHP[i]) == -1){
			// ERROR
			return 0;
		}
	}

	char buffer[255];
	int posInicial = 0;
	int aniosLista;
	// char** listaLineas = malloc(sizeof(char*) * cantidadAnios);
	for (i = 0; i < cantidadAnios; i++){
		pids[i] = fork();
		if (pids[i] == -1){
			printf("Error en la creacion del proceso\n");
			return 0;
		}else if (pids[i] == 0){
			//for(int k=0; k<cantidadAnios; k++){
				close(pipesPH[i][WRITE]);
				close(pipesHP[i][READ]);
			
			// Proceso hijo
			char buffer[255];
			int posicionInicial, posicionFinal;
			// printf("Proceso hijo %d creado\n", i);

			if (read(pipesPH[i][READ], buffer, sizeof(char) * 255) == -1){
				printf("Error en la lectura del pipe\n");
				return 0;
			}else{
				sscanf(buffer, "%d;%d", &posicionInicial, &posicionFinal);
				// proceso hijo recibe el mensaje del padre
				// Acá se le pasan las cabeceras a los procesos hijos
				// Se calcula la información segun las cabeceras posicionInicial y posicionFinal;
				int cantidadJuegosPorAnio = 0, cantidadGratis = 0;
				juego *juegosPorAnio = listaDeJuegosHijo(posicionInicial, posicionFinal, &cantidadJuegosPorAnio);

				int caro, barato, win, mac, lin;
				float promedio;
				//printf("anio: %d\n",juegosPorAnio[0].fecha);
				calculosDelHijo(juegosPorAnio, cantidadJuegosPorAnio, &caro, &barato, &promedio);
				//printf("cantidad %d anio %d bara %d, caro %d, promedio %f\n", cantidadJuegosPorAnio, juegosPorAnio[0].fecha, barato, caro, promedio);
				promedioPorPlataforma(juegosPorAnio, cantidadJuegosPorAnio, &win, &mac, &lin);
				//printf("anio %d win: %d, mac: %d, lin: %d\n", juegosPorAnio[0].fecha,win, mac, lin);
				//gratis=listaJuegosGratis(juegosPorAnio, cantidadJuegosPorAnio, &cantidadGratis, gratis);
				 //printf("%d.- gratis: \n", juegosPorAnio[0].fecha);
				/*for(int j=0;j<cantidadGratis;j++){
				printf("%s\n", juegosPorAnio[gratis[j]].nombre);
				} */
				char* bufferHijo = malloc(sizeof(char) * 5000);
				sprintf(bufferHijo, "%d;Año: %d\nJuego más caro: %s\nJuego más barato: %s\nPromedio de precios: %f\nWindows: %d Mac: %d Linux: %d\n",juegosPorAnio[0].fecha, juegosPorAnio[0].fecha, juegosPorAnio[caro].nombre, juegosPorAnio[barato].nombre, promedio, win, mac, lin);
				sprintf(bufferHijo, "%sJuegos gratis:\n", bufferHijo);
				for(int j=0;j<cantidadJuegosPorAnio;++j){
					//printf("%d\n",juegosPorAnio[j].gratis);
					if(juegosPorAnio[j].gratis == 1){
						sprintf(bufferHijo, "%s%s\n", bufferHijo, juegosPorAnio[j].nombre);
        			}
				}
				//printf("%d\n",cantidadGratis);
				for(int j=0;j<cantidadGratis;j++){
					//printf( "anio:%d, %d\n",juegosPorAnio[0].fecha,gratis[i]);
				}
				//printf("%s\n",bufferHijo);
				if (write(pipesHP[i][WRITE], bufferHijo, sizeof(char) * 5000) == -1){
					printf("estoy en el error\n");
					return 0;
				}
			
				return 0;
			}
			// printf("Proceso hijo %d recibio mensaje\n", i);
		}
	}

	// Proceso padre
	char bufferTotal[5000];
	
	for(int i=0;i<cantidadAnios;i++){
		close(pipesPH[i][READ]);
		close(pipesHP[i][WRITE]);
	sprintf(buffer, "%d;%d", posInicial, cabeceras[i]);
	posInicial = cabeceras[i];
	if (write(pipesPH[i][WRITE], buffer, sizeof(char) * 255) == -1){
		return 0;
	}
	}
	// Se espera a que terminen los procesos hijos
	FILE* archivoSalida = fopen(nombreSalida, "w");
	fclose(archivoSalida);
	for (i = 0; i < cantidadAnios; i++){
		waitpid(pids[i], NULL, 0);
		if (read(pipesHP[i][READ], bufferTotal, sizeof(char) * 5000) == -1){
			printf("estoy en el error\n");
			printf("Error en la lectura del pipe\n");
			return 0;
		}else{
			//printf("Bandera %d\n",bandera);
			char *token = strtok(bufferTotal, ";");
			aniosLista = atoi(token);
			
			if(aniosLista>= anioInicio){
				token = strtok(NULL, ";");
				if(bandera == 1){
					printf("%s\n",token);
				}
				escribirArchivoFinal(token,nombreSalida);
			}
			
			// token = strtok(NULL, ";");
			// strcpy(listaLineas[i], token);
		}
	}

	/*mergeSort2(aniosLista, 0, cantidadAnios);
	for(int i=0;i<cantidadAnios;i++){
		printf("%d\n", aniosLista[i]);
	}*/

	
	// Acá se le pasan las cabeceras a los procesos hijos

	// printf("%s", bufferTotal);
// string = "1999;Anio 1999:\n Juego mas caro"
// 	// strtok separa 2 datos por ;
// 	// guardamos los dos datos en 2 array distintos
// 	//[1999,1997,2000,2001]
// 	//["s1",s2]
// 	// ordenamos array de anios, y a la vez movemos los datos de el otro array
// 	// se escri

free(listaJuegos);
return 0;
}

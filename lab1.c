#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "padre.h"
#include "hijo.h"
#include "juego.h"

#define READ 0
#define WRITE 1

/*Función main que tiene el proceso del padre y de los hijos, primero se ponen leen las flag, luego se
crean los pipes, luego se crea el proceso padre y se leen los archivos, luego se crea el proceso hijo y se
leen los archivos según las entradas por anios de las cabeceras. Luego se cierran los pipes y se espera a
que los hijos terminen para finalmente escribir.*/
//entrada: Argc y argv
//salida: 0
int main(int argc, char *argv[]){
	int i;
	int option;
	char nombreEntrada[255];
	char nombreSalida[255];
	int anioInicio;
	float precioMinimo=0;
	int bandera =0;

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
	int *cabeceras = escribirJuego(archivoIntermedio, listaJuegos, cantidadJuegos, &cantidadAnios);
	
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
				int cantidadJuegosPorAnio = 0;
				juego *juegosPorAnio = listaDeJuegosHijo(posicionInicial, posicionFinal, &cantidadJuegosPorAnio);
				int contador = contadorJuegosPrecioMinimo(juegosPorAnio, cantidadJuegosPorAnio, precioMinimo);
				char* bufferHijo = malloc(sizeof(char) * 5000);
				//Si contador es 0, no hay juegos con precio mayor al minimo, por lo tanto se escribe solo los parámetros del año sin juegos, 
				//ni siquiera los juegos gratis
				if(contador==0){
					sprintf(bufferHijo, "%d;Año: %d\nJuego más caro:\nJuego más barato:\nPromedio de precios:\nWindows: 0 Mac: 0 Linux:0 \n",
					juegosPorAnio[0].fecha, juegosPorAnio[0].fecha);
					strcat(bufferHijo, "Juegos gratis:\n");		
				}else{
					int caro, barato, win, mac, lin;
					float promedio;
					calculosDelHijo(juegosPorAnio, cantidadJuegosPorAnio, &caro, &barato, &promedio,precioMinimo,contador);
					promedioPorPlataforma(juegosPorAnio, cantidadJuegosPorAnio, &win, &mac, &lin,precioMinimo,contador);
					sprintf(bufferHijo, "%d;Año: %d\nJuego más caro: %s\nJuego más barato: %s\nPromedio de precios: %f\nWindows: %d Mac: %d Linux: %d\n",
					juegosPorAnio[0].fecha, juegosPorAnio[0].fecha, juegosPorAnio[caro].nombre, juegosPorAnio[barato].nombre, promedio, win, mac, lin);
					strcat(bufferHijo, "Juegos gratis:\n");
					for(int j=0;j<cantidadJuegosPorAnio;++j){
						if(juegosPorAnio[j].gratis == 1){
							strcat(bufferHijo, juegosPorAnio[j].nombre);
							strcat(bufferHijo, "\n");
        				}
					}
				}
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
	FILE* archivoSalida = fopen(nombreSalida, "w");
	fclose(archivoSalida);
	for (i = 0; i < cantidadAnios; i++){
		waitpid(pids[i], NULL, 0);
		if (read(pipesHP[i][READ], bufferTotal, sizeof(char) * 5000) == -1){
			printf("estoy en el error\n");
			printf("Error en la lectura del pipe\n");
			return 0;
		}else{
			char *token = strtok(bufferTotal, ";");
			aniosLista = atoi(token);
			
			if(aniosLista>= anioInicio){
				token = strtok(NULL, ";");
				if(bandera == 1){
					printf("%s\n",token);
				}
				escribirArchivoFinal(token,nombreSalida);
			}
		}
	}
free(listaJuegos);
return 0;
}

//Lab 1 Sistemas operativos
//Integrantes: Leo Vergara Sepúlveda, Ian Rickmers Blamey
//Rut: 20.820.105-0, 20.284.545-2
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "padre.h"
#include "hijo.h"
#include "juego.h"
#include "otros.h"

#define READ 0
#define WRITE 1

/*Función main que tiene el proceso del padre y de los hijos, primero se ponen leen las flag, luego se
crean los pipes, luego se crea el proceso padre y se leen los archivos, luego se crea el proceso hijo y se
leen los archivos según las entradas por anios de las cabeceras. Luego se cierran los pipes y se espera a
que los hijos terminen para finalmente escribir.*/
//entrada: Argc y argv
//salida: 0
int main(int argc, char *argv[]){
	//Variables iniciales
	int i;
	int requeridoEntrada=0, requeridoSalida=0;
	int option;
	char nombreEntrada[255];
	char nombreSalida[255];
	int anioInicio=0;
	float precioMinimo=0;
	int bandera =0;

	//getopt para leer las flags
	while ((option = getopt(argc, argv,":i:o:d:p:b")) != -1) {
		switch (option) {
			case 'i' :
				requeridoEntrada = 1;
				strcpy(nombreEntrada,optarg);
				break;
			case 'o' :
				requeridoSalida = 1;
				strcpy(nombreSalida,optarg);
				break;
			case 'd' :
				if(atoi(optarg) < 0 || !verificarDecimal(optarg)){
					printf("Error: No se ingreso un año valido.\n");
					return 0;
				}
				anioInicio = atoi(optarg);
				break;
			case 'p' :
				if(atof(optarg) < 0 || !verificarDecimal(optarg)){
					printf("Error: No se ingreso un precio valido.\n");
					return 0;
				}
				precioMinimo = atof(optarg);
				break;
			case 'b' :
				bandera = 1;
				break;
			case '?' :
				printf("Flag no reconocida\n");
				return 0;
			case ':' :
				printf("Flag sin argumento\n");
				return 0;
		}
	} 

	if(requeridoEntrada == 0 || requeridoSalida == 0){
		printf("Error: Falta ingresar el nombre de los archivos de entrada y/o salida.\n");
		return 0;
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

	char buffer[1000];
	int posInicial = 0;
	int aniosLista;

	// Comienza el for que crea los procesos hijos con fork()
	for (i = 0; i < cantidadAnios; i++){
		pids[i] = fork();
		if (pids[i] == -1){
			printf("Error en la creacion del proceso\n");
			return 0;
		}else if (pids[i] == 0){

			// Codigo del hijo, primero se cierran los pipes
			close(pipesPH[i][WRITE]);
			close(pipesHP[i][READ]);
			char buffer[1000];
			int posicionInicial, posicionFinal;

			// Se lee el pipe de Padre Hijo, el cual contiene la posicion inicial y final de donde se tiene que colocar 
			// el proceso en el archivo intermedio
			if (read(pipesPH[i][READ], buffer, sizeof(char) * 1000) == -1){
				printf("Error en la lectura del pipe\n");
				return 0;
			}else{
				//Buffer que envia el mensaje del hijo al padre
				char* bufferHijo = (char*)malloc(sizeof(char) * 50000);
				// Se separa la posición inicial y final
				sscanf(buffer, "%d;%d", &posicionInicial, &posicionFinal);
				int cantidadJuegosPorAnio = 0;
				// Se lee el archivo intermedio y se guarda en un arreglo de juegos
				juego *juegosPorAnio = listaDeJuegosHijo(posicionInicial, posicionFinal, &cantidadJuegosPorAnio);
				// se cuentan los juegos que cumplen con el precio mínimo establecido
				int contador = contadorJuegosPrecioMinimo(juegosPorAnio, cantidadJuegosPorAnio, precioMinimo);

				//Si contador es 0, no hay juegos con precio mayor al minimo, por lo tanto se escribe solo los parámetros del año sin juegos, 
				//ni siquiera los juegos gratis
				if(contador==0){
					sprintf(bufferHijo, "%d,Año: %d\nJuego más caro:\nJuego más barato:\nPromedio de precios:\nWindows: 0 Mac: 0 Linux:0 \n",
					juegosPorAnio[0].fecha, juegosPorAnio[0].fecha);
					strcat(bufferHijo, "Juegos gratis:\n");		
				
				}else{

					//En otro caso se guardan todos los datos de los juegos que si cumplan el mínimo establecido
					int caro, barato;
					float win, mac, lin;
					float promedio;
					// Se calculan los datos de los juegos
					calculosDelHijo(juegosPorAnio, cantidadJuegosPorAnio, &caro, &barato, &promedio,precioMinimo,contador);
					promedioPorPlataforma(juegosPorAnio, cantidadJuegosPorAnio, &win, &mac, &lin,precioMinimo,contador);
					//Se guardan los datos en el buffer
					sprintf(bufferHijo, "%d,Año: %d\nJuego más caro: %s\nJuego más barato: %s\nPromedio de precios: %f\nWindows: %f Mac: %f Linux: %f\n",
					juegosPorAnio[0].fecha, juegosPorAnio[0].fecha, juegosPorAnio[caro].nombre, juegosPorAnio[barato].nombre, promedio, win, mac, lin);
					strcat(bufferHijo, "Juegos gratis:\n");
					for(int j=0;j<cantidadJuegosPorAnio;++j){
						if(juegosPorAnio[j].gratis == 1){
							strcat(bufferHijo, juegosPorAnio[j].nombre);
							strcat(bufferHijo, "\n");
        				}
					}
				}
				// Se escribe en el pipe de Hijo Padre el mensaje del buffer
				if (write(pipesHP[i][WRITE], bufferHijo, sizeof(char) * 50000) == -1){
					printf("estoy en el error\n");
					return 0;
				}
				// Se cierran los pipes y se libera la memoria
				close(pipesPH[i][READ]);
				close(pipesHP[i][WRITE]);
				free(bufferHijo);
				return 0;
			}
		}
	}

	// Proceso padre
	char bufferTotal[50000];
	
	// Se cierran los pipes y se escriben las posicion inicial y final para enviárselas al hijo
	for(int i=0;i<cantidadAnios;i++){
		close(pipesPH[i][READ]);
		close(pipesHP[i][WRITE]);

		sprintf(buffer, "%d;%d", posInicial, cabeceras[i]);
		posInicial = cabeceras[i];

		//Se envía el mensaje al hijo
		if (write(pipesPH[i][WRITE], buffer, sizeof(char) * 1000) == -1){
			return 0;
		}
	}

	// Crear el archivo de salida
	FILE* archivoSalida = fopen(nombreSalida, "w");
	if (archivoSalida == NULL)
    {
        printf("No se pudo abrir el archivo de salida\n");
        return 0;
    }
	fclose(archivoSalida);

	//Se lee el mensaje del hijo y se escribe en el archivo intermedio
	for (i = 0; i < cantidadAnios; i++){
		//Se espera a que terminen los hijos
		waitpid(pids[i], NULL, 0);
		//Se lee el mensaje
		if (read(pipesHP[i][READ], bufferTotal, sizeof(char) * 50000) == -1){
			printf("estoy en el error\n");
			printf("Error en la lectura del pipe\n");
			return 0;
		}else{
			//Se escribe en el archivo final
			char *token = strtok(bufferTotal, ",");
			aniosLista = atoi(token);
			
			if(aniosLista>= anioInicio){
				token = strtok(NULL, ",");
				if(bandera == 1){
					printf("%s\n",token);
				}
				escribirArchivoFinal(token,nombreSalida);
			}
		}
	}
	//Se libera la memoria
	free(listaJuegos);
	free(cabeceras);
	//Se cierran los pipes
	for(int i=0;i<cantidadAnios;i++){
		close(pipesPH[i][WRITE]);
		close(pipesHP[i][READ]);
	}
	return 0;
}

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "padre.c"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{	
	int i;
	int option;
	char nombreEntrada[255];
	char nombreSalida[255];
	int anioInicio;
	float precioMinimo;
	int bandera = 0;

	while((option = getopt(argc, argv, ":i:o:d:p:b")) != -1){
		switch(option){

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
	juego *listaJuegos=listaDeJuegos(nombreEntrada, &cantidadJuegos);

	// Se ordena con mergeSort
	mergeSort(listaJuegos,0,cantidadJuegos-1);

	// Se crea el archivo intermedio.
	int cantidadAnios = 0;
	char* archivoIntermedio = "intermedio.txt";
	int* cabeceras = escribirJuego(archivoIntermedio, listaJuegos, cantidadJuegos, &cantidadAnios);

	// Se genera un arreglo para almacenar los pids de cada proceso generado
    int pids[cantidadAnios];
    // Se definen un pipes para la comunicacion entre proceso padre y sus hijos
    int pipesPH[cantidadAnios][2]; // Comunicacion Padre ---> Hijo
    int pipesHP[cantidadAnios][2]; // Comunicacion Hijo ---> Padre

    // Se generan los pipe de Padre Hijo
    for(i = 0; i < cantidadAnios * 2; i++) {
        if(pipe(pipesPH[i]) == -1) {
            // ERROR
            return 0;
        }
    }   
    // Se generan los pipe de Hijo Padre
    for(i = 0; i < cantidadAnios * 2; i++) {
        if(pipe(pipesHP[i]) == -1) {
            // ERROR
            return 0;
        }
    }

	for (i = 0; i < cantidadAnios; i++)
    {
        pids[i] = fork();
        if (pids[i] == -1) {
            printf("Error en la creacion del proceso\n");
            return 0;
        }else if(pids[i] == 0){ 
			// Proceso hijo
			for (int j = 0; j < cantidadAnios; j++)
            {
                close(pipesPH[j][READ]);
                close(pipesHP[j][WRITE]);
            }
			char buffer[255];
			int posicionInicial, posicionFinal;
			while(1){
				if(read(pipesPH[i][READ], buffer, sizeof(char)*255)==-1){
					return 0;
				}
				else{
					if(strcmp(buffer,"END")==0){
						break;
					}
					char* token = strtok(buffer,";");
					posicionInicial = atoi(token);
					token = strtok(NULL,";");
					posicionFinal = atoi(token);
					printf("%d %d\n",posicionInicial, posicionFinal);
				}
			}
			//Se cierran los pipes de lectura de padre hijo y de escritura de hijo padre
            for (int j = 0; j < cantidadAnios; j++) {
                close(pipesPH[j][READ]);
                close(pipesHP[j][WRITE]);
            }
            return 0;
		}
	}

	// Proceso padre
	for(i = 0;i<cantidadAnios;i++){
		close(pipesPH[i][READ]);
        close(pipesHP[i][WRITE]);
	}
	char buffer[255];
	int posInicial = 0;
	for(i=0;i<cantidadAnios;i++){
		sprintf(buffer, "%d;%d", posInicial,cabeceras[i]);
		posInicial = cabeceras[i];
		if(write(pipesPH[i][WRITE], buffer, sizeof(char)*255)==-1){
			return 0;
		}
	}
	char bufferEnd[255] = "END";
	for(i=0;i<cantidadAnios;i++){
		if(write(pipesPH[i][WRITE], bufferEnd, sizeof(char)*255)==-1){
			return 0;
		}
	}

	free(listaJuegos);


	/* for(int j = 0; j < cantidadAnios; j++){
		printf("j:%d->%d\n", j,cabeceras[j]);
	}
	FILE* fichero = fopen(archivoIntermedio,"r");
	fseek(fichero, cabeceras[23], SEEK_SET);
	do{
		int car = fgetc(fichero);
		if(car != EOF){
			printf("%c",car);
		}
	}while(!feof(fichero));
 */
	// Se crean los procesos hijos por cada uno de los años
	// en el archivo.
	/*int pid;
	int***ArrayPipes = (int***)malloc(sizeof(int**)*n);
	for(int j=0;j<n;++j){
		ArrayPipes[j] = (int**)malloc(sizeof(int*)*2);
		for(int k=0;k<2;++k){
			ArrayPipes[j][k] = (int*)malloc(sizeof(int)*2);
			pipe(ArrayPipes[j][k]);
		}
	}
	int lectura;
	int escritura;
	for(int i=0;i<n;i++){
		pid = fork();
		if(pid>0){
			close(ArrayPipes[i][0][0]);
			close(ArrayPipes[i][1][1]);
		}
		else if(pid==0){
			close(ArrayPipes[i][0][1]);
			close(ArrayPipes[i][1][0]);
			break;
		}
		else{
			printf("Error al crear el proceso hijo");
			exit(0);
		}
	}*/
	return 0;
}

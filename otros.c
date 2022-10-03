#include <stdio.h>
void imprimir(int comienzo, int fin){
	FILE* fichero = fopen("intermedio.txt","r");
	fseek(fichero, comienzo, SEEK_SET);
	int pos;
	do{
		int car = fgetc(fichero);
		pos = ftell(fichero);
		if(car != EOF){
			printf("%c",car);
		}
		if(pos == fin){
			break;
		}
	}while(!feof(fichero));
}

/* 
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
#include <stdio.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int option;
	char *nombreEntrada;
	char *nombreSalida;
	int anioInicio;
	int precioMinimo;
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
			sscanf(optarg, "%d", &precioMinimo);
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
	return 0;
}

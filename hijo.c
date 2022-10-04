#include "hijo.h"

//Lee el archivo intermedio y crea una lista de juegos por cada hijo
//Entrada: comienzo, fin del año y puntero al tamaño de la lista
//Salida: lista de juegos
juego *listaDeJuegosHijo(int comienzo, int fin, int *i)
{
    // Se abre el archivo escrito por el padre.
    FILE *archivo = fopen("intermedio.txt", "r");
    // Se posiciona el puntero al comienzo del año perteneciente al proceso.
    fseek(archivo,comienzo,SEEK_SET);
    int pos = ftell(archivo);
    // Si no existe el archivo, se retorna NULL.
    if (archivo == NULL){
        printf("No se pudo abrir el archivo");
        return NULL;
    }
    char linea[2000];

    //Se cuentan las lineas del archivo
    while (fgets(linea, 2000, archivo) != NULL)
    {
        //Se cuenta cada linea
        *i = *i + 1;
        //Se posiciona el puntero al final de cada linea.
        pos = ftell(archivo);
        //Si el puntero llega al final, se sale del ciclo.
        if(pos == fin){
            break;
        }
        
    }

    //Se crea un arreglo de juegos con la cantidad de lineas
    //contadas anteriormente.
    juego *lista = (juego *)malloc(sizeof(juego) * (*i));
    //Se posiciona el puntero al comienzo del año perteneciente al proceso.
    fseek(archivo,comienzo,SEEK_SET);
    pos = ftell(archivo);
    //Se lee el archivo y se guarda en el arreglo de estructuras
    int j = 0;
    while (j < *i)
    {
        //Se lee cada linea del archivo
        fgets(linea, 2000, archivo);
        //tokenizamos la linea
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
    pos = ftell(archivo);
    // Se cierra el archivo.
    fclose(archivo);
    return lista;
}

//Calcula los promedios, el más caro y el más barato de los juegos por cada hijo
//Entrada: lista de juegos, tamaño de la lista, punteros a los promedios, el más caro, el más barato
//         el precio minimo y cantidad de juegos mayor al precio minimo.
//Salida: nada
void calculosDelHijo(juego* juegos, int n, int *c, int *b, float *prom, float precioMinimo, int contador){
    //Se inicializan los precios caros, baratos y una sumatoria para el promedio
    int caro = juegos[0].precio;
    int barato = juegos[0].precio;
    float suma = 0;
    for(int i=0 ;i<n;i++){
        //Se verfica el precio más caro
        if(juegos[i].precio > caro && juegos[i].precio >= precioMinimo){
            caro = juegos[i].precio;
            *c = i;
        }
        //Se verifica el precio más barato
        else if(juegos[i].precio < barato && juegos[i].precio >= precioMinimo){
            barato = juegos[i].precio;
            *b = i;
        }
        //Se suma el precio de cada juego mayor al precio minimo para calcular el promedio
        if(juegos[i].precio >= precioMinimo){
            suma = suma + juegos[i].precio;
        } 
    }
    //Se calcula el promedio
    *prom = suma/contador;
    return ;

}

//Calcula la cobertura de cada plataforma por año
//Entrada: lista de juegos, tamaño de la lista, punteros a los porcentaje de cada plataforma,
//         el precio minimo y cantidad de juegos mayor al precio minimo.
//Salida: nada
void promedioPorPlataforma(juego* juegos, int n, int *w, int *m, int *l, float precioMinimo, int contador){
    //Se inicializan los contadores de cada plataforma
    int win = 0;
    int mac = 0;
    int lin = 0;
    for(int i=0 ;i<n;i++){
        //Se verifica si el juego tiene la plataforma y si es mayor al precio minimo
        if(juegos[i].win == 1 && juegos[i].precio >= precioMinimo){
            win++;
        }
        if(juegos[i].mac == 1 && juegos[i].precio >= precioMinimo){
            mac++;
        }
        if(juegos[i].lin == 1 && juegos[i].precio >= precioMinimo){
            lin++;
        }
    }
    //Se calcula el porcentaje de cada plataforma
    *w = (win/contador)*100;
    *m = (mac/contador)*100;
    *l = (lin/contador)*100;
    return;
}

//Calcula la cantidad de juegos con precio mayores al precio minimo por año
//Entrada: lista de juegos, tamaño de la lista, y precio minimo
//Salida: cantidad de juegos mayores al precio minimo
int contadorJuegosPrecioMinimo(juego* juegos, int n, int precioMinimo){
    //Se inicializa el contador
    int contador = 0;
    for(int i=0;i<n;i++){
        //Se verifica si el precio del juego es mayor al precio minimo
        if(juegos[i].precio >= precioMinimo){
            contador++;
        }  
    }
    return contador;
}



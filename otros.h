#include <stdio.h>
#include <string.h>
#include <ctype.h>

int verificarDecimal(char * cadena){
    int i;
    int contador=0;
    for(i=0;i<strlen(cadena);i++){
        if(!isdigit(cadena[i])){
            if(cadena[i]=='.' && contador==0){
                contador++;
            }else{
                return 0;
            }
        }
    }
    return 1;
}
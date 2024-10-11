#include <stdio.h>

int main(int argc, char **argv){
    if(argc < 2){
        return 1;
    }
    float avg = 0;
    for(int j = 1; j<argc;j++){
        float nombre;
        int nb_success = sscanf(argv[j],"%e",&nombre);
        if(nb_success < 1){
            puts("Impossible");
            return 2;
        }
        if(nombre<0 || nombre>20){
            puts("Impossible");
            return 3;
        }
        avg += nombre;
    }
    avg = avg / (argc-1);
    printf("%2.2f\n",avg);
}
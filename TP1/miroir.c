#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){
    if(argc < 2){
        return 1;
    }
    for(int j = 1; j<argc;j++){
        int len = strlen(argv[j]);
        char result[len];

        result[len-1] = '\0';
        for(int i = len; i>=0;i--){
            result[len-i] = argv[j][i-1];
        }
        printf("%s\n",result);
    }
}
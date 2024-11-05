#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

int xy_to_index(int x, int y, size_t width)
{
    return (y * 3 * width) + (3 * x);
}

char* readline(int fd)
{
    int offset = 0;
    size_t size;
    char dummy;
    do {
        size = read(fd,&dummy,1);
        offset += size;
    } while(size != 0 && dummy != '\n' && dummy != EOF);

    char* line = malloc(offset * sizeof(char)+1);
    lseek(fd,-offset,SEEK_CUR);
    read(fd,line,offset);
    line[offset]='\0';
    return line;
}

void load_image(char* filepath, unsigned char** image_data, size_t* width, size_t* height)
{
    int fd = open(filepath,O_RDONLY);
    if (fd == -1)
    {
        perror("Can't open input file");
        exit(-1);
    }
    char* line = readline(fd);
    if ( strncmp("P6", line, 2) != 0)
    {
        fputs("Input File is not a ppm file\n", stderr);
        free(line);
        exit(-1);
    }
    free(line);
    do {
        line = readline(fd);
        if (line[0] == '#')
        {
            free(line);
            continue;
        }
        int success = sscanf(line,"%lu %lu",width,height);
        if (success != 2)
        {
            fputs("Input File is not a valid ppm file\n", stderr);
            free(line);
            exit(-1);
        }
        free(line);
    } while(*width == 0);
    int dummy = 0;
    do {
        line = readline(fd);
        if (line[0] == '#')
        {
            free(line);
            continue;
        }
        int success = sscanf(line,"%d",&dummy);
        if (success != 1)
        {
            fputs("Input File is not a valid ppm file\n", stderr);
            free(line);
            exit(-1);
        }
        free(line);
    } while(dummy == 0);

    unsigned data_size = 3 * (*width) * (*height);
    *image_data = malloc(data_size * sizeof(unsigned char));

    read(fd,*image_data,3*(*width)*(*height));
}

void save_image(char* filepath, unsigned char* image, size_t width, size_t height)
{
    int fd = open(filepath,O_CREAT|O_WRONLY,0644);
    if (fd == -1)
    {
        perror("Can't open output file");
        exit(-1);
    }
    write(fd,"P6\n",3);
    char* size = NULL;
    asprintf(&size,"%lu %lu\n",width, height);
    write(fd,size,strlen(size));
    free(size);
    write(fd,"255\n",4);
    write(fd,image,3*width*height);
    write(fd,"\n",1);
    close(fd);

}

void calcul(unsigned char* image,unsigned char* out_image, int width, int height, int x, int y)
{
    assert(x >= 0);
    assert(x < width);
    assert(y >= 0);
    assert(y < height);

    int idx = xy_to_index(x,y,width);
    int red = image[idx];
    int green = image[idx+1];
    int blue = image[idx+2];
    int new_red = 0;
    int new_green = 0;
    int new_blue = 0;
    for (int i = 0; i <= 1000; i++)
    {
        float fact = 393.0f / i;
        new_red =   fact*red + 0.769*green + 0.189*blue;
        fact = 686.0f/i;
        new_green = 0.349*red + fact*green + 0.168*blue;
        fact = 131.0f/i;
        new_blue =  0.272*red + 0.534*green + fact*blue;
    }
    out_image[idx] = new_red <=  255 ? new_red : 255;
    out_image[idx+1] = new_green <= 255 ? new_green  : 255;
    out_image[idx+2] = new_blue <= 255 ? new_blue : 255;
}

/*----- Zone à Modifier -----*/

/*----- Fin de Zone à Modifier -----*/

int main(void)
{
    unsigned char* image = NULL;
    unsigned char* out_image = NULL;
    size_t width = 0;
    size_t height = 0;

    //chargement de l'image
    load_image("image.ppm", &image, &width, &height);
    //creation d'une image vide de taille identique à l'image d'origine
    out_image = malloc((width*height*3) * sizeof(unsigned char));

    /*----- Zone à Modifier -----*/
    //calcul de chacun des pixels d'origine pour remplir les pixels de l'image de sortie
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            calcul(image, out_image, width, height, x, y);
        }
    }
    /*----- Fin de Zone à Modifier -----*/
    //sauvegarde de l'image
    save_image("sepia.ppm", out_image, width, height);
    //libération de la mémoire
    free(image);
    free(out_image);
    return 0;
}

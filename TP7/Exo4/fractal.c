#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

const int size = 1000;
const int tailleRegions = 5;
int coordonnesBase[3] = {0,0,0};

struct pixels{
	pthread_mutex_t mutex;
	unsigned char *image;
};

static void calcul(int x, int y, unsigned char *pixel)
{
	long double p_x = (long double)x/size * 2.0l - 1.0l;
	long double p_y = (long double)y/size * 2.0l - 1.0l;
	long double tz = 0.7;
	long double zoo = powl(0.5l, 13.0l * tz);
	long double cc_x = -0.05l + p_x * zoo;
	long double cc_y = 0.6805l + p_y * zoo;
	long double z_x = 0.0l;
	long double z_y = 0.0l;
	long double m2 = 0.0l;
	long double co = 0.0l;
	long double dz_x = 0.0l;
	long double dz_y = 0.0l;
	int i;
	for (i = 0; i < 2560; i++)
	{
		long double old_dz_x, old_z_x;
		if (m2 > 1024.0l)
			break;
		// Z' -> 2�Z�Z' + 1
		old_dz_x = dz_x;
		dz_x = 2.0l * z_x * dz_x - z_y * dz_y + 1.0l;
		dz_y = 2.0l * z_x * dz_y + z_y * old_dz_x;
		// Z -> Z� + c
		old_z_x = z_x;
		z_x = cc_x + z_x * z_x - z_y * z_y;
		z_y = cc_y + 2.0l * old_z_x * z_y;
		m2 = z_x * z_x + z_y * z_y;
		co += 1.0l;
	}
	// distance	
	// d(c) = |Z|�log|Z|/|Z'|
	long double d = 0.0l;
	if (co < 2560.0l)
	{
		long double dot_z = z_x * z_x + z_y * z_y;
		d = sqrtl(dot_z/(dz_x*dz_x+dz_y*dz_y)) * logl(dot_z);
		pixel[0] = fmodl(co,256.0l);
		d = 4.0l * d / zoo;
		if (d < 0.0l) d = 0.0l;
		if (d > 1.0l) d = 1.0l;
		pixel[1] = fmodl((1.0l-d) * 255.0l * 300.0l, 255.0l);
		d = powl(d, 50.0l*0.25l);
		pixel[2] = d * 255.0l;
	}
	else
		pixel[0]=pixel[1]=pixel[2]=0;
}

void *generate(void* args){
	struct pixels *argument = args;
	while (1)
	{
		pthread_mutex_lock(&argument->mutex);
		if(coordonnesBase[2] == 1){
			pthread_mutex_unlock(&argument->mutex);
			pthread_exit(0);
		}
		int xMin = coordonnesBase[0];
		int yMin = coordonnesBase[1];
		if(xMin==size-tailleRegions && yMin == size-tailleRegions){
			coordonnesBase[2] = 1;
		}
		coordonnesBase[0] = (coordonnesBase[0]+tailleRegions)%size;
		if(coordonnesBase[0] == 0){
			coordonnesBase[1] = (coordonnesBase[1]+tailleRegions)%size;
		}
		pthread_mutex_unlock(&argument->mutex);
		for(int y = yMin; y<yMin+tailleRegions; y++){
			for(int x = xMin; x<xMin+tailleRegions; x++){
				calcul(x,y,argument->image+3*(y * size + x));
			}
		}
	}
	
	// printf("%d %d\n", argument->yMin, argument->yMax);
}

int main(int argc, char const *argv[])
{
	if(argc < 2){
		exit(-1);
	}
	int N = atoi(argv[1]);
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);

	FILE *file;

	// pthread_t lignes[N];
	unsigned char *image = malloc(3*size*size);
	pthread_t thread[N];
	struct pixels pixels[N];
	for(int i = 0; i<N;i++){
		
	}

	for(int i = 0; i<N;i++){
		pixels[i].image = image;
		pixels[i].mutex = mutex;
		pthread_create(&thread[i],NULL,generate,&pixels[i]);
	}
	for(int i = 0; i<N;i++){
		pthread_join(thread[i], NULL);
	}

	// écriture fichier
	file = fopen("image.ppm", "w");
	fprintf(file, "P6\n%d %d\n255\n", size, size);
	fwrite(image, size, size * 3, file);
	fclose(file);
	free(image);

	return 0;
}

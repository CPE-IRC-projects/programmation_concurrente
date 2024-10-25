// gpu.c

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

#include "gpu.h"

#define SIZE 1000
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

struct IVec2
{
  int x, y;
};

struct RasterState
{
  struct Triangle tri;
  struct Vec3 AB, BC, CA;
  int xmin, xmax, ymin, ymax;
  int x, y;
  int eof;
};

unsigned char image[3 * SIZE * SIZE];
float depths[SIZE * SIZE];

// GL context
struct Vec3 current_ambiant;
struct Vec3 current_diffuse;

/********************************************************************************/
static void clear_image(void)
{
  int i;
  memset(image, 0, sizeof image);
  for (i = 0; i < SIZE * SIZE; i++)
    depths[i] = -10.0f;
}

/********************************************************************************/
static void save_image(void)
{
  FILE *file = fopen("image.ppm", "w");
  fprintf(file, "P6\n%d %d\n255\n", SIZE, SIZE);
  if (fwrite(image, 3, SIZE * SIZE, file) != SIZE * SIZE)
  {
    perror("Cannot write image");
  }
  fclose(file);
}

struct TriangleState
{
  int index;
  float *v;
  float *n;
  int   *t;
  int    nt;
};

const struct Vec3 light_dir = { 25.0, 5.0, 10.0 };
static struct Vec3 light_dir_norm;
float dot(struct Vec3 a, struct Vec3 b)
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void *triangle_creator_init(void)
{
  struct TriangleState *st = malloc(sizeof *st);
  clear_image();

  st->index = 0;
  // Init shading stuff
  float norm = sqrt(dot(light_dir, light_dir));
  light_dir_norm.x = light_dir.x / norm;
  light_dir_norm.y = light_dir.y / norm;
  light_dir_norm.z = light_dir.z / norm;
  if (1)
  {
    FILE *jummy = fopen("elephant.obj", "r");
    // first pass, counting
    int nb_vertices = 0;
    int nb_normals = 0;
    while (!feof(jummy))
    {
      char buf[1000];
      fgets(buf, 1000, jummy);
      float dum;
      int idum;
      if (sscanf(buf, "v %f %f %f\n",&dum,&dum,&dum)==3)nb_vertices++;
      if (sscanf(buf, "vn %f %f %f\n",&dum,&dum,&dum)==3)nb_normals++;
      if (sscanf(buf, "f %d//%d %d//%d %d//%d\n", &idum, &idum, &idum, &idum, &idum, &idum)==6)st->nt++;
    }
    st->v = malloc((nb_vertices + 1) * 3 * sizeof(float));
    st->n = malloc((nb_normals  + 1) * 3 * sizeof(float));
    st->t = malloc(st->nt * 3 * sizeof(int));
    rewind(jummy);
    int rv=1, rn=1, rt=0;
    while (!feof(jummy))
    {
      char buf[1000];
      fgets(buf, 1000, jummy);
      int idum;
      if (sscanf(buf, "v %f %f %f\n", &st->v[rv*3+0], &st->v[rv*3+1], &st->v[rv*3+2])==3)rv++;
      if (sscanf(buf, "vn %f %f %f\n",&st->n[rn*3+0], &st->n[rn*3+1], &st->n[rn*3+2])==3)rn++;
      if (sscanf(buf, "f %d//%d %d//%d %d//%d\n", &st->t[rt*3+0], &idum, &st->t[rt*3+1], &idum, &st->t[rt*3+2], &idum)==6)rt++;
    }
  }
  return st;
}
// coroutine stuff
#define BEGIN static int state=0; switch(state) { case 0:
#define RETURN(x) do { state=__LINE__; return x; case __LINE__:; } while (0)
#define FINISH }

/********************************************************************************/
  struct Vec3 center[4] = { { 0.5, 0.5, -2.5 }, {-0.5, 0.5, -2.5 }, {-0.5,-0.5, -2.5 }, { 0.5,-0.5, -2.5 } };
  float radius[4] = { 0.45, 0.45, 0.45, 0.45 };
  struct Vec3 ambient[4] = { { 0.2, 0.0, 0.0 }, { 0.0, 0.2, 0.0 }, { 0.0, 0.0, 0.2 }, { 0.2, 0.2, 0.0 } };
  struct Vec3 diffuse[4] = { { 0.8, 0.0, 0.0 }, { 0.0, 0.8, 0.0 }, { 0.0, 0.0, 0.8 }, { 0.8, 0.8, 0.0 } };
int triangle_creator_get_next_triangle(void *creator, struct Triangle *triangle)
{
  const int nb_long = 50;
  const int nb_latt = 20;
  static int i, j, odd, obj;
  struct TriangleState *st = creator;
  struct Vec3 elephant_ambiant = { 0.3, 0.2, 0.2 };
  struct Vec3 elephant_diffuse = { 0.7, 0.5, 0.5 };
  BEGIN;
  current_ambiant = elephant_ambiant;
  current_diffuse = elephant_diffuse;
  for (i = 0; i < st->nt; i++)
  {
    for (j = 0; j < 3; j++)
    {
      float angle = 60 * M_PI / 180.0;
      float mat[9] = { cos(angle), 0, sin(angle), 0, 1, 0, -sin(angle), 0, cos(angle) };
      float x = st->v[3*st->t[3*i+j]+0];
      float y = st->v[3*st->t[3*i+j]+1];
      float z = st->v[3*st->t[3*i+j]+2];
      float rx = mat[3*0+0] * x + mat[3*1+0] * y + mat[3*2+0] * z;
      float ry = mat[3*0+1] * x + mat[3*1+1] * y + mat[3*2+1] * z;
      float rz = mat[3*0+2] * x + mat[3*1+2] * y + mat[3*2+2] * z;
      triangle->vertices[j].position.x = SIZE * (1.0 + 1.5 * rx) / 2.0;
      triangle->vertices[j].position.y = SIZE * (1.0 - 1.5 * ry) / 2.0;
      triangle->vertices[j].position.z =               1.5 * rz;
      struct Vec3 f;
      f.x = st->n[3*st->t[3*i+j]+0];
      f.y = st->n[3*st->t[3*i+j]+1];
      f.z = st->n[3*st->t[3*i+j]+2];
      triangle->vertices[j].inter1 = MAX(0.0, dot(light_dir_norm, f));
    }
    triangle->sync = 0;
    RETURN(1);
  }
  triangle->sync = 1;
  RETURN(1);
  for (obj = 0; obj < 4; obj++)
  {
    current_ambiant = ambient[obj];
    current_diffuse = diffuse[obj];
    for (i = 0-20; i < nb_long-20; i++)
    {
      for (j = 0; j < nb_latt; j++)
      {
        for (odd = 0; odd < 2; odd++)
        {
          struct Vec3 f;
          f.y = cos((j+1) * M_PI/nb_latt);
          f.x = sin((j+1) * M_PI/nb_latt) * cos(i * M_PI * 2.0 / nb_long);
          f.z = sin((j+1) * M_PI/nb_latt) * sin(i * M_PI * 2.0 / nb_long);
          triangle->vertices[1].position.x = SIZE * (f.x * radius[obj] + center[obj].x + 1.0)/2.0;
          triangle->vertices[1].position.y = SIZE * (1.0 - f.y * radius[obj] - center[obj].y)/2.0;
          triangle->vertices[1].position.z = f.z* radius[obj] +center[obj].z;
          triangle->vertices[1].inter1 = MAX(0.0, dot(light_dir_norm, f));

          f.y = cos(j * M_PI/nb_latt);
          f.x = sin(j * M_PI/nb_latt) * cos((i+1) * M_PI * 2.0 / nb_long);
          f.z = sin(j * M_PI/nb_latt) * sin((i+1) * M_PI * 2.0 / nb_long);
          triangle->vertices[2].position.x = SIZE * (f.x * radius[obj] + center[obj].x + 1.0)/2.0;
          triangle->vertices[2].position.y = SIZE * (1.0 - f.y * radius[obj] - center[obj].y)/2.0;
          triangle->vertices[2].position.z = f.z* radius[obj] +center[obj].z;
          triangle->vertices[2].inter1 = MAX(0.0, dot(light_dir_norm, f));

          f.y = cos((j+odd) * M_PI/nb_latt);
          f.x = sin((j+odd) * M_PI/nb_latt) * cos((i+odd) * M_PI * 2.0 / nb_long);
          f.z = sin((j+odd) * M_PI/nb_latt) * sin((i+odd) * M_PI * 2.0 / nb_long);
          triangle->vertices[0].position.x = SIZE * (f.x * radius[obj] + center[obj].x + 1.0)/2.0;
          triangle->vertices[0].position.y = SIZE * (1.0 - f.y * radius[obj] - center[obj].y)/2.0;
          triangle->vertices[0].position.z = f.z* radius[obj] +center[obj].z;
          triangle->vertices[0].inter1 = MAX(0.0, dot(light_dir_norm, f));

          triangle->sync = 0;
          RETURN(1);
        }
      }
    }
    triangle->sync = 1;
    RETURN(1);
  }
  FINISH;
  return 0;
}

/********************************************************************************/
void  triangle_creator_destroy(void *creator)
{
  save_image();
  free(creator);
}

/********************************************************************************/
int triangle_is_sync(struct Triangle *triangle)
{
  return triangle->sync;
}
/********************************************************************************/
/******************** Le rasterizer *********************************************/
/********************************************************************************/
void *raster_init(const struct Triangle *tri)
{
  int k;
  struct RasterState *state = malloc(sizeof*state);
  state->tri = *tri;

  state->xmin = state->ymin = SIZE - 1; state->xmax = state->ymax = 0;
  for (k = 0; k < 3; k++)
  {
    state->xmin = MIN(state->xmin, state->tri.vertices[k].position.x);
    state->ymin = MIN(state->ymin, state->tri.vertices[k].position.y);
    state->xmax = MAX(state->xmax, state->tri.vertices[k].position.x);
    state->ymax = MAX(state->ymax, state->tri.vertices[k].position.y);
  }
  state->x = state->xmin = MAX(state->xmin,0);
  state->y = state->ymin = MAX(state->ymin,0);
  state->xmax = MIN(state->xmax, SIZE - 1);
  state->ymax = MIN(state->ymax, SIZE - 1);

  state->AB.x = state->tri.vertices[1].position.x - state->tri.vertices[0].position.x;
  state->AB.y = state->tri.vertices[1].position.y - state->tri.vertices[0].position.y;
  state->BC.x = state->tri.vertices[2].position.x - state->tri.vertices[1].position.x;
  state->BC.y = state->tri.vertices[2].position.y - state->tri.vertices[1].position.y;
  state->CA.x = state->tri.vertices[0].position.x - state->tri.vertices[2].position.x;
  state->CA.y = state->tri.vertices[0].position.y - state->tri.vertices[2].position.y;
  state->eof = 0;
  return state;
}

#define RBEGIN_CONTEXT struct context { int line
#define R

/********************************************************************************/
int raster_get_next_fragment(void *raster_state, struct Fragment *fragment)
{
  struct RasterState *s = raster_state;
  int found = 0;

  if (s->eof) return 0;
  while (1)
  {
    struct Vec3 Ax, Bx, Cx;
    float det[3];
    Ax.x = s->x - s->tri.vertices[0].position.x;
    Ax.y = s->y - s->tri.vertices[0].position.y;
    Bx.x = s->x - s->tri.vertices[1].position.x;
    Bx.y = s->y - s->tri.vertices[1].position.y;
    Cx.x = s->x - s->tri.vertices[2].position.x;
    Cx.y = s->y - s->tri.vertices[2].position.y;
    det[0] = s->AB.x*Ax.y-s->AB.y*Ax.x;
    det[1] = s->BC.x*Bx.y-s->BC.y*Bx.x;
    det[2] = s->CA.x*Cx.y-s->CA.y*Cx.x;
    if ((det[0] <= 0 && det[1] <= 0 && det[2] <= 0)|| (det[0] >=0 && det[1] >= 0 && det[2] >= 0))
    {
      int sum = det[0]+det[1]+det[2];
      if (sum != 0)
      {
        float a[3];

        fragment->x = s->x;
        fragment->y = s->y;
        a[2] = (float)(det[0])/sum;
        a[0] = (float)(det[1])/sum;
        a[1] = (float)(det[2])/sum;
        fragment->inter1 = a[0] * s->tri.vertices[0].inter1 + a[1] * s->tri.vertices[1].inter1 + a[2] * s->tri.vertices[2].inter1;
        fragment->depth = a[0] * s->tri.vertices[0].position.z+ a[1] * s->tri.vertices[1].position.z+a[2]* s->tri.vertices[2].position.z;
        fragment->sync = 0;

        found = 1;
      }
    }
    s->x++;
    if (s->x > s->xmax)
    {
      s->x = s->xmin;
      s->y++;
      if (s->y > s->ymax)
      {
        s->eof = 1;
        if (!found) return 0;
      }
    }
    if (found) return 1;
  }
}

void raster_destroy(void *raster_state)
{
  free(raster_state);
}
/********************************************************************************/
struct Fragment fragment_sync(void)
{
  struct Fragment fragment;
  fragment.sync = 1;
  return fragment;
}

/********************************************************************************/
int fragment_is_sync(struct Fragment *fragment)
{
  return fragment->sync;
}

/********************************************************************************/
/******************** Le shader *************************************************/
/********************************************************************************/
static pthread_mutex_t shader_mutex = PTHREAD_MUTEX_INITIALIZER;
void gpu_shader(struct Fragment fragment)
{
  int off = fragment.y * SIZE + fragment.x;
  pthread_mutex_lock(&shader_mutex);
  if (fragment.depth > depths[off])
  {
    depths[off] = fragment.depth;
    image[3 * off + 0] = (current_ambiant.x + fragment.inter1 * current_diffuse.x) * 255;
    image[3 * off + 1] = (current_ambiant.y + fragment.inter1 * current_diffuse.y) * 255;
    image[3 * off + 2] = (current_ambiant.z + fragment.inter1 * current_diffuse.z) * 255;
  }
  pthread_mutex_unlock(&shader_mutex);
}

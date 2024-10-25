// gpu.h

#ifndef GPU_H
#define GPU_H

struct Vec3
{
  float x, y, z;
};

struct Vertex
{
  struct Vec3 position;
  float inter1;
  float inter2;
  float inter3;
  //...
};

struct Triangle
{
  int sync;
  struct Vertex vertices[3];
};

struct Fragment
{
  int sync;
  int x, y;
  float depth;
  float inter1;
  float inter2;
  float inter3;
};

// Les fonctions relatives aux triangles
void *triangle_creator_init(void);
int   triangle_creator_get_next_triangle(void *creator, struct Triangle *triangle);
void  triangle_creator_destroy(void *creator);
int triangle_is_sync(struct Triangle *triangle);

// Les fonctions relatives aux fragments
void *raster_init(const struct Triangle *tri);
int   raster_get_next_fragment(void *raster_state, struct Fragment *fragment);
void  raster_destroy(void *raster_state);
struct Fragment fragment_sync(void);
int fragment_is_sync(struct Fragment *fragment);

// La fonction de shading.
void gpu_shader(struct Fragment fragment);

#endif

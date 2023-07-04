#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include <stdbool.h>

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

vec3_t cube_vertices[N_CUBE_VERTICES];
face_t cube_faces[N_CUBE_FACES];

typedef struct
{
    vec3_t *vertices; // dynamic array of vertices
    face_t *faces;    // dynamic array of faces
    vec3_t rotation;  // rotation of mesh
} mesh_t;

extern mesh_t mesh;

extern void load_cube_mesh_data(void);
extern bool load_obj_file_data(const char *filename);

#endif

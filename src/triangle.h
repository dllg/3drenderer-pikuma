#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

typedef struct
{
    int a;
    int b;
    int c;
} face_t;

#define N_TRIANGLE_POINTS 3
typedef struct
{
    vec2_t points[N_TRIANGLE_POINTS];
} triangle_t;

#endif

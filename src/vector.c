#include <math.h>
#include "vector.h"

// Implementation of Vector 2d functions
float vec2_length(vec2_t v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b)
{
    return (vec2_t){a.x + b.x, a.y + b.y};
}

vec2_t vec2_sub(vec2_t a, vec2_t b)
{
    return (vec2_t){a.x - b.x, a.y - b.y};
}

vec2_t vec2_mul(vec2_t v, float factor)
{
    return (vec2_t){v.x * factor, v.y * factor};
}

vec2_t vec2_div(vec2_t v, float factor)
{
    return (vec2_t){v.x / factor, v.y / factor};
}

float vec2_dot(vec2_t a, vec2_t b)
{
    return a.x * b.x + a.y * b.y;
}

void vec2_normalize(vec2_t *v)
{
    float length = vec2_length(*v);
    v->x /= length;
    v->y /= length;
}


// Implemetation of Vector 3d functions
float vec3_length(vec3_t v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b)
{
    return (vec3_t){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3_t vec3_sub(vec3_t a, vec3_t b)
{
    return (vec3_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3_t vec3_mul(vec3_t v, float factor)
{
    return (vec3_t){v.x * factor, v.y * factor, v.z * factor};
}

vec3_t vec3_div(vec3_t v, float factor)
{
    return (vec3_t){v.x / factor, v.y / factor, v.z / factor};
}

vec3_t vec3_cross(vec3_t a, vec3_t b)
{
    vec3_t cross_product = {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x};
    return cross_product;
}

float vec3_dot(vec3_t a, vec3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void vec3_normalize(vec3_t *v)
{
    float length = vec3_length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)};
    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)};
    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z};
    return rotated_vector;
}

vec4_t vec4_from_vec3(vec3_t v)
{
    return (vec4_t){v.x, v.y, v.z, 1};
}

vec3_t vec3_from_vec4(vec4_t v)
{
    return (vec3_t){v.x, v.y, v.z};
}

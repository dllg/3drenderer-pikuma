#include "matrix.h"
#include <math.h>

mat4_t mat4_identity(void)
{
    // | 1 0 0 0 |
    // | 0 1 0 0 |
    // | 0 0 1 0 |
    // | 0 0 0 1 |
    mat4_t m = {{{1, 0, 0, 0},
                 {0, 1, 0, 0},
                 {0, 0, 1, 0},
                 {0, 0, 0, 1}}};
    return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
    // | sx 0 0 0 |
    // | 0 sy 0 0 |
    // | 0 0 sz 0 |
    // | 0 0 0  1 |
    mat4_t m = mat4_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;
    return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
    // | 1 0 0 tx |
    // | 0 1 0 ty |
    // | 0 0 1 tz |
    // | 0 0 0  1 |
    mat4_t m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    return m;
}

mat4_t mat4_make_rotation_x(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    // | 1  0  0  0 |
    // | 0  c -s  0 |
    // | 0  s  c  0 |
    // | 0  0  0  1 |
    mat4_t m = mat4_identity();
    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;
    return m;
}

mat4_t mat4_make_rotation_y(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    // | c  0  s  0 |
    // | 0  1  0  0 |
    // |-s  0  c  0 |
    // | 0  0  0  1 |
    mat4_t m = mat4_identity();
    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;
    return m;
}

mat4_t mat4_make_rotation_z(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    // | c -s 0 0 |
    // | s  c 0 0 |
    // | 0  0 1 0 |
    // | 0  0 0 1 |
    mat4_t m = mat4_identity();
    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;
    return m;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar)
{
    float f = 1.0f / tan(fov / 2.0f);
    // | A  0  0  0 |
    // | 0  f  0  0 |
    // | 0  0  B  C |
    // | 0  0  1  0 |
    // A = aspect * f
    // B = zfar / (zfar - znear)
    // C = (-zfar * znear) / (zfar - znear)
    mat4_t m = {{{0}}};
    m.m[0][0] = aspect * f;
    m.m[1][1] = f;
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-zfar * znear) / (zfar - znear);
    m.m[3][2] = 1.0; // Store z value in w for perspective divide
    return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
    return (vec4_t){
        m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
        m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
        m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
        m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w};
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b)
{
    mat4_t m = {0};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }
    return m;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v)
{
    // Multiply the original vector with the projection matrix
    vec4_t v_transformed = mat4_mul_vec4(mat_proj, v);

    // Perform perspective divide with the original z value that now is stored in w
    if (v_transformed.w != 0.0f)
    {
        v_transformed.x /= v_transformed.w;
        v_transformed.y /= v_transformed.w;
        v_transformed.z /= v_transformed.w;
    }
    return v_transformed;
}

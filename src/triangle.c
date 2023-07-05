#include "display.h"
#include "triangle.h"

void int_swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Draw a filled triangle with the flat-bottom method
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Find the slope of the left and right edges of the triangle
    float inv_sloap1 = (x1 - x0) / (float)(y1 - y0);
    float inv_sloap2 = (x2 - x0) / (float)(y2 - y0);

    float x_start = x0;
    float x_end = x0;

    // Loop all the scanlines from top to bottom
    for (int y = y0; y <= y2; ++y)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start += inv_sloap1;
        x_end += inv_sloap2;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // Find the slope of the left and right edges of the triangle
    float inv_sloap1 = (x2 - x0) / (float)(y2 - y0);
    float inv_sloap2 = (x2 - x1) / (float)(y2 - y1);

    float x_start = x2;
    float x_end = x2;

    // Loop all the scanlines from bottom to top
    for (int y = y2; y >= y0; --y)
    {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inv_sloap1;
        x_end -= inv_sloap2;
    }
}

// Draw a filled triangle with the flat-top/flat-bottom method
// https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // We need to sort the vertices by y-coordinate ascending
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }
    if (y1 > y2)
    {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }
    if (y0 > y1)
    {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    if (y1 == y2)
    {
        // We can simply draw the flat-bottom triangle
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1)
    {
        // We can simply draw the flat-top triangle
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        // Calculate the new vertex (Mx, My)
        int My = y1;
        int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

        fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
        fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
    }
}

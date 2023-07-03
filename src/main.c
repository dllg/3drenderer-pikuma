#include "display.h"
#include "vector.h"

// Declare area of vectors
const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS]; // 9x9x9 cube
vec2_t projected_points[N_POINTS];

float fov_factor = 128;

void setup(void)
{
    // Allocate memory for color buffer
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

    // Create a SDL texture to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);

    int point_count = 0;

    // Start loading my area of vectors
    // From negative -1 to 1 (in this 9x9x9 cube)
    for (float x = -1; x <= 1; x += 0.25f)
    {
        for (float y = -1; y <= 1; y += 0.25f)
        {
            for (float z = -1; z <= 1; z += 0.25f)
            {
                vec3_t new_point = {.x = x, .y = y, .z = z};
                cube_points[point_count++] = new_point;
            }
        }
    }
}

bool process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        return false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            return false;
        break;
    default:
        break;
    }
    return true;
}

// Function that receives a 3d vector and returns a projected 2d point
vec2_t project(vec3_t point)
{
    vec2_t projected_point = {
        .x = fov_factor * point.x,
        .y = fov_factor * point.y
    };
    return projected_point;
}

void update(void)
{
    for (int i = 0; i < N_POINTS; i++)
    {
        vec3_t point = cube_points[i];

        // Project the current point
        vec2_t projected_point = project(point);

        // Save the projected 2d vector in the array of projected points
        projected_points[i] = projected_point;
    }
}

void render(void)
{
    // Loop all projected points and render them
    for (int i = 0; i < N_POINTS; i++)
    {
        vec2_t projected_point = projected_points[i];
        draw_rect(
            projected_point.x + (window_width / 2),
            projected_point.y + (window_height / 2),
            4,
            4,
            0xFFFFFF00);
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(void)
{
    bool is_running = initialize_window();

    setup();

    // vec3_t myvector = { 2.0, 3.0, -4.0 };

    while (is_running)
    {
        is_running = process_input();
        update();
        render();
    }

    destroy_window();
    return 0;
}

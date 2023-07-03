#include "display.h"

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

void update(void)
{
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid(100, 0xFF000000);
    draw_rect(300, 200, 300, 150, 0xFFFF00FF);
    draw_pixel(20, 20, 0xFFFFFF00);

    render_color_buffer();
    clear_color_buffer(0xFFFFFF00);

    SDL_RenderPresent(renderer);
}

int main(void)
{
    bool is_running = initialize_window();

    setup();

    while (is_running)
    {
        is_running = process_input();
        update();
        render();
    }

    destroy_window();
    return 0;
}
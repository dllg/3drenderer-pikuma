#include "array.h"
#include "display.h"
#include "light.h"
#include "vector.h"
#include "matrix.h"
#include "mesh.h"

triangle_t *triangles_to_render = NULL;
vec3_t camera_position = {0, 0, 0};
mat4_t proj_matrix;

int previous_frame_time = 0;

bool draw_points = false;
bool draw_wireframe = true;
bool draw_filled_triangles = true;
bool enable_backface_culling = true;

void setup(void)
{
    // Initalize render mode and triangle culling method
    render_method = RENDER_WIRE;
    cull_method = CULL_BACKFACE;

    // Allocate memory for color buffer
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

    // Create a SDL texture to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);

    load_cube_mesh_data();
    load_obj_file_data("./assets/cube.obj");

    // Initialize projection matrix
    float fov = M_PI / 3.0f; // The same as 180/3 = 60 degrees
    float aspect = (float)window_height / (float)window_width;
    float znear = 0.1f;
    float zfar = 100.0f;
    proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);
}

bool process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_MOUSEMOTION:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            mesh.rotation.x += event.motion.yrel / 100.0f;
            mesh.rotation.y -= event.motion.xrel / 100.0f;
        }
        break;
    case SDL_QUIT:
        return false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            return false;
        if (event.key.keysym.sym == SDLK_1)
            render_method = RENDER_WIRE_VERTEX;
        if (event.key.keysym.sym == SDLK_2)
            render_method = RENDER_WIRE;
        if (event.key.keysym.sym == SDLK_3)
            render_method = RENDER_FILL_TRIANGLE;
        if (event.key.keysym.sym == SDLK_4)
            render_method = RENDER_FILL_TRIANGLE_WIRE;
        if (event.key.keysym.sym == SDLK_c)
            cull_method = CULL_BACKFACE;
        if (event.key.keysym.sym == SDLK_d)
            cull_method = CULL_NONE;
        if (event.key.keysym.sym == SDLK_a)
            mesh.scale.x += 0.01f;
        if (event.key.keysym.sym == SDLK_z)
            mesh.scale.x -= 0.01f;
        if (event.key.keysym.sym == SDLK_UP)
            mesh.translation.y -= 0.1f;
        if (event.key.keysym.sym == SDLK_DOWN)
            mesh.translation.y += 0.1f;
        if (event.key.keysym.sym == SDLK_LEFT)
            mesh.translation.x -= 0.1f;
        if (event.key.keysym.sym == SDLK_RIGHT)
            mesh.translation.x += 0.1f;
        break;
    default:
        break;
    }
    return true;
}

void update(void)
{
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }
    previous_frame_time = SDL_GetTicks();

    // Initialize triangles to render
    triangles_to_render = NULL;

    mesh.translation.z = 5.0;
    mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
    mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
    mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);

    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; i++)
    {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec4_t transformed_vertices[3];

        // Loop all 3 vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++)
        {
            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            // Create a World Matrix to scale, rotate and translate our original vertices
            mat4_t world_matrix = mat4_identity();
            // Order matters: scale -> rotate -> translate
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

            // Multiply World Matrix with the transformed vertex
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

            // Save transformed vertex in array of transformed vertices
            transformed_vertices[j] = transformed_vertex;
        }

        // Get the normal vector of the face
        vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); /*   A   */
        vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); /*  / \  */
        vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); /* C---B */

        // Get the vector subtraction of B-A and C-A
        vec3_t vector_ab = vec3_sub(vector_b, vector_a);
        vec3_t vector_ca = vec3_sub(vector_c, vector_a);
        vec3_normalize(&vector_ab);
        vec3_normalize(&vector_ca);

        // Compute the face normal using the cross product to find the vector perpendicular
        vec3_t normal = vec3_cross(vector_ab, vector_ca); // Left-handed coordinate system
        vec3_normalize(&normal);

        // Check backface culling
        if (cull_method == CULL_BACKFACE)
        {
            // Find the vector between the camera and the first vertex of the face
            vec3_t camera_ray = vec3_sub(camera_position, vector_a);

            // Check how aligned my normal is with the camera ray
            float dot_normal_camera = vec3_dot(normal, camera_ray);

            // Bypass the triangle that are looking away from the camera
            if (dot_normal_camera < 0)
                continue;
        }

        // Loop all 3 vertices and perform the projection transformation
        vec4_t projected_points[3];
        for (int j = 0; j < 3; j++)
        {
            // Project the current vertex
            projected_points[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

            // Scale into the view
            projected_points[j].x *= (window_width / 2.0);
            projected_points[j].y *= (window_height / 2.0);

            // Invert the y values to account for flipped screen y coordinates
            projected_points[j].y *= -1;

            // Translate projected points to the middle of the screen
            projected_points[j].x += (window_width / 2.0);
            projected_points[j].y += (window_height / 2.0);
        }

        // Calculate the shade intensity based on how aligned the light ray is with the normal vector
        float light_intensity_factor = -vec3_dot(normal, light.direction);
        uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intensity_factor);

        triangle_t projected_triangle = {
            .points = {
                {.x = projected_points[0].x, .y = projected_points[0].y},
                {.x = projected_points[1].x, .y = projected_points[1].y},
                {.x = projected_points[2].x, .y = projected_points[2].y}},
            .color = triangle_color,
            .avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0f};
        // Save the projected triangle in the array of triangles to render
        array_push(triangles_to_render, projected_triangle);
    }
}

// sort the triangles on depth
void sort(void)
{
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles - 1; i++)
    {
        for (int j = 0; j < num_triangles - i - 1; j++)
        {
            if (triangles_to_render[j].avg_depth < triangles_to_render[j + 1].avg_depth)
            {
                triangle_t temp = triangles_to_render[j];
                triangles_to_render[j] = triangles_to_render[j + 1];
                triangles_to_render[j + 1] = temp;
            }
        }
    }
}

void render(void)
{
    sort(); // Sort the triangles to render by depth

    // Loop all projected points and render them
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++)
    {
        triangle_t triangle = triangles_to_render[i];
        if (render_method == RENDER_WIRE_VERTEX)
        {
            draw_rect(triangle.points[0].x - 2, triangle.points[0].y - 2, 4, 4, 0xFFFF0000);
            draw_rect(triangle.points[1].x - 2, triangle.points[1].y - 2, 4, 4, 0xFFFF0000);
            draw_rect(triangle.points[2].x - 2, triangle.points[2].y - 2, 4, 4, 0xFFFF0000);
        }
        if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
        {
            draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
                                 triangle.points[1].x, triangle.points[1].y,
                                 triangle.points[2].x, triangle.points[2].y,
                                 triangle.color);
        }
        if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE)
        {
            draw_triangle(triangle.points[0].x, triangle.points[0].y,
                          triangle.points[1].x, triangle.points[1].y,
                          triangle.points[2].x, triangle.points[2].y,
                          0xFFAAAAAA);
        }
    }

    // Clear triangles to render array every frame loop
    array_free(triangles_to_render);

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void free_resources(void)
{
    free(color_buffer);
    array_free(mesh.faces);
    array_free(mesh.vertices);
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
    free_resources();

    return 0;
}

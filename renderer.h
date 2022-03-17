#include "chip8.h"

#include <stdint.h>
#include <SDL2/SDL.h>

#ifndef CHIP8_EMULATOR_RENDERER_H
#define CHIP8_EMULATOR_RENDERER_H

#define RENDERER_WINDOW_TITLE "cheb's amazing chip8-emulator"
#define RENDERER_SCALE 10

typedef struct {
    uint32_t pixel_buff[VIDEO_WIDTH*VIDEO_HEIGHT];

    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    SDL_Texture *sdl_texture;
}Renderer;

Renderer *new_renderer();

//init and close SDL2 stuff
void rendering_init();
void rendering_close();

void renderer_close(Renderer *renderer);

void renderer_update(Renderer *renderer);

//TODO: renderer is not needed
void fetch_input_events(ChipIO *events);

//TODO: make this private
void renderer_copy_pixels_from_vram(uint8_t *vram, uint32_t* pixel_buf);

#endif //CHIP8_EMULATOR_RENDERER_H

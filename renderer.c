#include "renderer.h"
#include "chip8.h"

#include <stdlib.h>

void renderer_copy_pixels_from_vram(uint8_t *vram, uint32_t* pixel_buf){
    for(int i=0;i<(VIDEO_WIDTH*VIDEO_HEIGHT);i++){
        uint8_t index = i>>3;
        uint8_t sub_index = i&7;
        pixel_buf[i] = ((vram[index]&(0x80>>sub_index))>>(7-sub_index)) *0x00ffffff;
    }
}

void rendering_init(){
    SDL_Init(SDL_INIT_VIDEO);
}
void rendering_close(){
    SDL_Quit();
}

void renderer_update(Renderer *renderer){
    SDL_UpdateTexture(renderer->sdl_texture, NULL, renderer->pixel_buff, VIDEO_WIDTH*sizeof(uint32_t));

    SDL_RenderCopy(renderer->sdl_renderer, renderer->sdl_texture, NULL, NULL);
    SDL_RenderPresent(renderer->sdl_renderer);
}

void renderer_close(Renderer *renderer){
    SDL_DestroyTexture(renderer->sdl_texture);
    SDL_DestroyRenderer(renderer->sdl_renderer);
    SDL_DestroyWindow(renderer->sdl_window);
    free(renderer);
}

Renderer *new_renderer(){
    Renderer *renderer = calloc(1,sizeof(Renderer));
    renderer->sdl_window = SDL_CreateWindow(RENDERER_WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIDEO_WIDTH * RENDERER_SCALE, VIDEO_HEIGHT * RENDERER_SCALE, SDL_WINDOW_RESIZABLE);
    renderer->sdl_renderer = SDL_CreateRenderer(renderer->sdl_window,-1,0);
    renderer->sdl_texture = SDL_CreateTexture(renderer->sdl_renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
    return renderer;
}
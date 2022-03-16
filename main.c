#include "chip8.h"
#include "renderer.h"
#include "input.h"

#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char **argv) {
    if(argc<2){
        printf("usage: chip-emulator <path-to-rom>\n");
        exit(1);
    }
    char *path = argv[1];
    FILE *file = fopen(path,"rb");
    if (!file){
        fprintf(stderr, "could not open file\n");
        exit(1);
    }
    fseek(file,0,SEEK_END);
    size_t file_size = ftell(file);
    fseek(file,0,SEEK_SET);
    uint8_t *file_buffer = malloc(file_size);
    fread(file_buffer, 1, file_size, file);
    fclose(file);

    Chip8 *chip = new_chip8(file_buffer, file_size, CH8_VERBOSE | CH8_USE_8XNN);
    free(file_buffer); //chip copied everything needed!

    InputEvents inputs;
    memset(&inputs,0,sizeof inputs);

    rendering_init();
    Renderer *renderer = new_renderer();

    uint32_t old_ticks = SDL_GetTicks();
    while(!inputs.quit){
        fetch_input_events(&inputs);

        uint32_t new_ticks = SDL_GetTicks();
        chip->timer = (chip->timer-(new_ticks-old_ticks))%61;
        chip->sound_timer = (chip->sound_timer-(new_ticks-old_ticks))%61;
        old_ticks = new_ticks;

        if(inputs.await_key && (!inputs.controls))
            continue;
        else
            inputs.await_key = 0;

        renderer_copy_pixels_from_vram(chip->vram, renderer->pixel_buff);
        renderer_update(renderer);
        chip8_cycle(chip,&inputs);
        if(!inputs.turbo)
            SDL_Delay(2);
    }
    renderer_close(renderer);
    rendering_close();
    return 0;
}

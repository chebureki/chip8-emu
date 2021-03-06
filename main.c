#include "chip8.h"
#include "renderer.h"
#include "input.h"

#include <stdio.h>

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

    Inputs inputs = {.chipio = {}, .extra=0};

    rendering_init();
    Renderer *renderer = new_renderer();

    char paused = 0;

    uint32_t old_ticks = SDL_GetTicks();
    while(!(inputs.extra&INPUT_QUIT)){
        fetch_input_events(&inputs);
        if(inputs.extra&INPUT_PAUSE)
            paused = 1;
        if(inputs.extra&INPUT_RUN)
            paused = 0;

        uint32_t new_ticks = SDL_GetTicks();
        chip->timer = (chip->timer-(new_ticks-old_ticks))%61;
        chip->sound_timer = (chip->sound_timer-(new_ticks-old_ticks))%61;
        old_ticks = new_ticks;

        if(inputs.chipio.await_key && (!inputs.chipio.controls))
            continue;
        else
            inputs.chipio.await_key = 0;

        if (!paused || (inputs.extra&INPUT_STEP)){
            chip8_cycle(chip,&inputs.chipio);
            if(inputs.extra&INPUT_STEP) inputs.extra &= ~INPUT_STEP; //unset step key so only one step is made
        }


        if(inputs.chipio.drawn_to_display) {
            renderer_copy_pixels_from_vram(chip->vram, renderer->pixel_buff);
            renderer_update(renderer);
            inputs.chipio.drawn_to_display = 0;
        }
        if(!(inputs.extra&INPUT_TURBO))
            SDL_Delay(1);
    }
    chip8_close(chip);
    renderer_close(renderer);
    rendering_close();
    return 0;
}

#include "input.h"
void fetch_input_events(Inputs *inputs) {
    SDL_Event sdl_event;
    SDL_PollEvent(&sdl_event);
    switch (sdl_event.type) {
        case SDL_QUIT:
            inputs->extra|=INPUT_QUIT;
            break;

        case SDL_KEYDOWN:
            switch(sdl_event.key.keysym.sym){
                case SDLK_TAB:  inputs->extra |= INPUT_TURBO; break;

                case SDLK_1: inputs->chipio.controls|=KEY_1;break;
                case SDLK_2: inputs->chipio.controls|=KEY_2;break;
                case SDLK_3: inputs->chipio.controls|=KEY_3;break;
                case SDLK_4: inputs->chipio.controls|=KEY_C;break;
                case SDLK_q: inputs->chipio.controls|=KEY_4;break;
                case SDLK_w: inputs->chipio.controls|=KEY_5;break;
                case SDLK_e: inputs->chipio.controls|=KEY_6;break;
                case SDLK_r: inputs->chipio.controls|=KEY_D;break;
                case SDLK_a: inputs->chipio.controls|=KEY_7;break;
                case SDLK_s: inputs->chipio.controls|=KEY_8;break;
                case SDLK_d: inputs->chipio.controls|=KEY_9;break;
                case SDLK_f: inputs->chipio.controls|=KEY_E;break;
                case SDLK_y: inputs->chipio.controls|=KEY_A;break;
                case SDLK_x: inputs->chipio.controls|=KEY_0;break;
                case SDLK_c: inputs->chipio.controls|=KEY_B;break;
                case SDLK_v: inputs->chipio.controls|=KEY_F;break;
            }
            break;
        case SDL_KEYUP:
            switch(sdl_event.key.keysym.sym){
                case SDLK_TAB: inputs->extra&=~INPUT_TURBO;

                case SDLK_1: inputs->chipio.controls&=~KEY_1;break;
                case SDLK_2: inputs->chipio.controls&=~KEY_2;break;
                case SDLK_3: inputs->chipio.controls&=~KEY_3;break;
                case SDLK_4: inputs->chipio.controls&=~KEY_C;break;
                case SDLK_q: inputs->chipio.controls&=~KEY_4;break;
                case SDLK_w: inputs->chipio.controls&=~KEY_5;break;
                case SDLK_e: inputs->chipio.controls&=~KEY_6;break;
                case SDLK_r: inputs->chipio.controls&=~KEY_D;break;
                case SDLK_a: inputs->chipio.controls&=~KEY_7;break;
                case SDLK_s: inputs->chipio.controls&=~KEY_8;break;
                case SDLK_d: inputs->chipio.controls&=~KEY_9;break;
                case SDLK_f: inputs->chipio.controls&=~KEY_E;break;
                case SDLK_y: inputs->chipio.controls&=~KEY_A;break;
                case SDLK_x: inputs->chipio.controls&=~KEY_0;break;
                case SDLK_c: inputs->chipio.controls&=~KEY_B;break;
                case SDLK_v: inputs->chipio.controls&=~KEY_F;break;
            }
            break;
    }
}
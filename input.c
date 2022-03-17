#include "input.h"
void fetch_input_events(ChipIO *events) {
    SDL_Event sdl_event;
    SDL_PollEvent(&sdl_event);
    switch (sdl_event.type) {
        case SDL_QUIT:
            events->quit=1;
            break;

        case SDL_KEYDOWN:
            switch(sdl_event.key.keysym.sym){
                case SDLK_TAB: events->turbo=1;

                case SDLK_1: events->controls|=KEY_1;break;
                case SDLK_2: events->controls|=KEY_2;break;
                case SDLK_3: events->controls|=KEY_3;break;
                case SDLK_4: events->controls|=KEY_C;break;
                case SDLK_q: events->controls|=KEY_4;break;
                case SDLK_w: events->controls|=KEY_5;break;
                case SDLK_e: events->controls|=KEY_6;break;
                case SDLK_r: events->controls|=KEY_D;break;
                case SDLK_a: events->controls|=KEY_7;break;
                case SDLK_s: events->controls|=KEY_8;break;
                case SDLK_d: events->controls|=KEY_9;break;
                case SDLK_f: events->controls|=KEY_E;break;
                case SDLK_y: events->controls|=KEY_A;break;
                case SDLK_x: events->controls|=KEY_0;break;
                case SDLK_c: events->controls|=KEY_B;break;
                case SDLK_v: events->controls|=KEY_F;break;
            }
            break;
        case SDL_KEYUP:
            switch(sdl_event.key.keysym.sym){
                case SDLK_TAB: events->turbo=0;

                case SDLK_1: events->controls&=~KEY_1;break;
                case SDLK_2: events->controls&=~KEY_2;break;
                case SDLK_3: events->controls&=~KEY_3;break;
                case SDLK_4: events->controls&=~KEY_C;break;
                case SDLK_q: events->controls&=~KEY_4;break;
                case SDLK_w: events->controls&=~KEY_5;break;
                case SDLK_e: events->controls&=~KEY_6;break;
                case SDLK_r: events->controls&=~KEY_D;break;
                case SDLK_a: events->controls&=~KEY_7;break;
                case SDLK_s: events->controls&=~KEY_8;break;
                case SDLK_d: events->controls&=~KEY_9;break;
                case SDLK_f: events->controls&=~KEY_E;break;
                case SDLK_y: events->controls&=~KEY_A;break;
                case SDLK_x: events->controls&=~KEY_0;break;
                case SDLK_c: events->controls&=~KEY_B;break;
                case SDLK_v: events->controls&=~KEY_F;break;
            }
            break;
    }
}
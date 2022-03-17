#include "chip8.h"
#include <SDL2/SDL.h>

#ifndef CHIP8_EMULATOR_INPUT_H
#define CHIP8_EMULATOR_INPUT_H

#define INPUT_TURBO_KEY SDLK_TAB
#define INPUT_PAUSE_KEY SDLK_b
#define INPUT_RUN_KEY SDLK_n
#define INPUT_STEP_KEY SDLK_m

#define INPUT_QUIT 1<<0
#define INPUT_TURBO 1<<1
#define INPUT_PAUSE 1<<2
#define INPUT_RUN 1<<3
#define INPUT_STEP 1<<4

typedef struct {
    ChipIO chipio;
    uint16_t extra;
}Inputs;

void fetch_input_events(Inputs *inputs);
#endif //CHIP8_EMULATOR_INPUT_H

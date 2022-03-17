#include "chip8.h"
#include <SDL2/SDL.h>

#ifndef CHIP8_EMULATOR_INPUT_H
#define CHIP8_EMULATOR_INPUT_H

#define INPUT_QUIT 1<<0
#define INPUT_TURBO 1<<1
#define INPUT_PAUSE

typedef struct {
    ChipIO chipio;
    uint16_t extra;
}Inputs;

void fetch_input_events(Inputs *inputs);
#endif //CHIP8_EMULATOR_INPUT_H

#include <stdint.h>

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H

#define USER_PROGRAM_START 0x200

#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

#define STACK_CAP 48

#define KEY_0 1<<0
#define KEY_1 1<<1
#define KEY_2 1<<2
#define KEY_3 1<<3
#define KEY_4 1<<4
#define KEY_5 1<<5
#define KEY_6 1<<6
#define KEY_7 1<<7
#define KEY_8 1<<8
#define KEY_9 1<<9
#define KEY_A 1<<10
#define KEY_B 1<<11
#define KEY_C 1<<12
#define KEY_D 1<<13
#define KEY_E 1<<14
#define KEY_F 1<<15

typedef struct {

    uint16_t controls;
    uint8_t await_key;

    //TODO: this is out ouf place
    uint8_t quit;
    uint8_t turbo;
}ChipIO;

#define CH8_EXIT_ON_ERROR 1<<0
#define CH8_VERBOSE 1<<1
#define CH8_USE_8XNN 1<<2

typedef struct{
    uint16_t config;

    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t index_register;
    uint16_t stack[STACK_CAP];
    uint8_t stack_pos;
    uint16_t program_counter;
    uint8_t timer;
    uint8_t sound_timer;

    //monochrome 64*32 resolution
    uint8_t vram[VIDEO_WIDTH*VIDEO_HEIGHT/8];
}Chip8;



void chip8_cycle(Chip8 *chip8, InputEvents* input);
Chip8 *new_chip8(uint8_t *program_data, uint16_t program_data_length, uint16_t config);
void chip8_close();

#endif //CHIP8_EMULATOR_CHIP8_H

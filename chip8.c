#include "chip8.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TIMER_MAX 60
#define FONT_BUFFER_OFFSET 0x50
#define FONT_BUFFER_SIZE 0x50

uint8_t font_buffer[FONT_BUFFER_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init_font(Chip8 *chip){
    memcpy(chip->memory+FONT_BUFFER_OFFSET, font_buffer, FONT_BUFFER_SIZE);
}

uint8_t chip8_get_pixel(Chip8 *chip, uint16_t x, uint16_t y){
    uint16_t index = (VIDEO_WIDTH*y + x)>>3;
    uint8_t sub_index = (VIDEO_WIDTH*y + x)&7;
    return (chip->vram[index]&(0x80>>sub_index))>>(7-sub_index);
}

uint8_t chip8_set_pixel(Chip8 *chip, uint16_t x, uint16_t y, uint8_t val){
    uint16_t index = (VIDEO_WIDTH*y + x)>>3;
    uint8_t sub_index = (VIDEO_WIDTH*y + x)&7;
    chip->vram[index] = (chip->vram[index] & ~(0x80>>sub_index)) | (val<<(7-sub_index))  ;
    return 0;
}

int16_t chip8_stack_pop(Chip8 *chip){
    return chip->stack[--chip->stack_pos];
}

void chip8_stack_push(Chip8 *chip, int16_t val){
    chip->stack[chip->stack_pos++] = val;
}

Chip8 *new_chip8(uint8_t *program_data, uint16_t program_data_length, uint16_t config) {
    Chip8 *chip = calloc(1,sizeof(Chip8));
    chip->config = config;
    chip8_init_font(chip);
    chip->timer=TIMER_MAX;
    chip->sound_timer=TIMER_MAX;

    chip->program_counter = USER_PROGRAM_START;
    memcpy(chip->memory+USER_PROGRAM_START,program_data,program_data_length);
    return chip;
}

void chip8_close(Chip8* chip){
    free(chip);
}

//instructions are 2 bytes long
uint16_t chip8_fetch(Chip8 *chip){
    return (((uint16_t)chip->memory[chip->program_counter])<<8) | chip->memory[chip->program_counter+1];
}

void chip8_clear_screen(Chip8 *chip){
    memset(chip->vram,0,VIDEO_WIDTH*VIDEO_HEIGHT/8);
}

//TODO: this is an quite lengthy implementation
void chip8_display_instruction(Chip8 *chip, uint8_t x, uint8_t y, uint8_t height){
    chip->registers[0xf] = 0;
    for(int i=0;i<height;i++){
        for(int j=0;j<8;j++) {
            //do not weirdly wrap
            if((x+j)>=VIDEO_WIDTH)
                break;
            uint8_t prev_bit = chip8_get_pixel(chip, x + j, y + i);
            uint8_t curr_bit = (chip->memory[chip->index_register + i] & (0x80 >> j)) >> (7 - j);
            chip8_set_pixel(chip, x + j, y + i, curr_bit^prev_bit);
            chip->registers[0xf] |= prev_bit&&curr_bit; //simplified from: prev_bit && !(curr_bit^prev_bit);
        }
    }
}

void chip8_cycle(Chip8 *chip, ChipIO* input){
    if(input->await_key)
        fprintf(stderr, "cycle execution despite awaited input\n");

    uint16_t ins = chip8_fetch(chip);

    if(chip->config&CH8_VERBOSE)
        printf("pc: %04x ins: %04x\n",chip->program_counter,ins);
    chip->program_counter+=2;
    uint16_t NNN =   (ins&0x0fff)>>0;    //e.g.: 1NNN
    uint8_t NN =    (ins&0x00ff)>>0;    //e.g.: 6XNN
    uint8_t N =     (ins&0x000f)>>0;    //e.g.: DXYN
    uint8_t X =     (ins&0x0f00)>>8;    //e.g.: 7XNN
    uint8_t Y =     (ins&0x00f0)>>4;    //e.g.: 8XY0

    switch ((ins &0xf000) >> 12) {
        case 0:
            switch (ins) {
                case 0x00E0: chip8_clear_screen(chip);return;
                case 0x00EE: chip->program_counter = chip8_stack_pop(chip); return;
                default:
                    if(chip->config&CH8_VERBOSE)
                        fprintf(stderr, "ignoring machine code routine %04x\n", NNN);
                    return;
            }
            break;
        //1NNN
        case 1: chip->program_counter = NNN; return;

        //2NNN
        case 2: chip8_stack_push(chip,chip->program_counter);chip->program_counter = NNN; return;

        //3XNN
        case 3: if(chip->registers[X] == NN) chip->program_counter+=2;return;

        //4XNN
        case 4: if(chip->registers[X] != NN) chip->program_counter+=2;return;

        //5XY0
        //TODO: verify last 4 bits are equal to 0
        case 5: if(chip->registers[X] == chip->registers[Y]) chip->program_counter+=2;return;

        //6XNN
        case 6: chip->registers[X] = NN; return;

        //7XNN
        case 7: chip->registers[X]+=NN; return;

        case 8:
            //N is the right most nibble
            switch (N) {
                case 0: chip->registers[X] =chip->registers[Y]; return;
                case 1: chip->registers[X]|=chip->registers[Y]; return;
                case 2: chip->registers[X]&=chip->registers[Y]; return;
                case 3: chip->registers[X]^=chip->registers[Y]; return;
                case 4: {
                    uint16_t a = (uint16_t)chip->registers[X]+(uint16_t)chip->registers[Y];
                    chip->registers[0xf] = a>0xff;
                    chip->registers[X] = (uint8_t) a;
                    return;
                }
                case 5: {
                    int16_t a = (int16_t)chip->registers[X]-(int16_t)chip->registers[Y];
                    chip->registers[0xf] = a>=0;
                    chip->registers[X] = (uint8_t) a;
                    return;
                }
                case 6:  chip->registers[0xf] = chip->registers[X]&1; chip->registers[X]>>=1; return;
                case 7: {
                    int16_t a = (int16_t)chip->registers[Y]-(int16_t)chip->registers[X];
                    chip->registers[0xf] = a>=0;
                    chip->registers[X] = (uint8_t) a;
                    return;
                }
                case 0xe:  chip->registers[0xf] = (chip->registers[X]&0x80)>>7; chip->registers[X]<<=1; return;
            }
            break;

        //9XY0
        //TODO: verify last 4 bits are equal to 0
        case 9: if(chip->registers[X] != chip->registers[Y]) chip->program_counter+=2;return;

        //ANNN
        case 0xa: chip->index_register=NNN; return;

        //BXNN / BNNN
        case 0xb:{
            if(chip->config|CH8_USE_8XNN)
                chip->program_counter+=chip->registers[0]+NNN;
            else
                chip->program_counter+=chip->registers[X]+NNN;
            return;
        }

        //TODO: seed rng
        //CXNN
        case 0xc: chip->registers[X] = (uint8_t)(rand()&0xff)&NN;return;

        //DXYN
        case 0xd: chip8_display_instruction(chip,chip->registers[X],chip->registers[Y],N); input->drawn_to_display=1;return;

        case 0xe:
            switch (NN) {
                case 0x9e: if(input->controls&(1<<chip->registers[X])) chip->program_counter+=2;return;
                case 0xa1: if(!(input->controls&(1<<chip->registers[X]))) chip->program_counter+=2;return;
            }
            break;

        case 0xf:
            switch (NN) {
                case 0x07: chip->registers[X]=chip->timer; return;
                case 0x0a: input->await_key=1;printf("awaiting input\n");return;
                case 0x15: chip->timer = chip->registers[X]; return;
                case 0x18: chip->sound_timer = chip->registers[X]; return;
                case 0x1e: chip->index_register += chip->registers[X]; return;
                case 0x29: chip->index_register = FONT_BUFFER_OFFSET+(chip->registers[X]*5); return;
                case 0x33: {
                    int acc = chip->registers[X];
                    for(int i=2;i>=0;i--){
                        chip->memory[chip->index_register+i] = acc%10;
                        acc/=10;
                    }
                    return;
                }

                case 0x55:{
                    for(int i=0;i<=X;i++)
                        chip->memory[chip->index_register+i] = chip->registers[i];
                    return;
                }
                case 0x65:{
                    for(int i=0;i<=X;i++)
                        chip->registers[i] = chip->memory[chip->index_register+i];
                    return;
                }
            }
            break;
    }
    fprintf(stderr,"unhandled instruction %04x\n",ins);
    if(chip->config&CH8_EXIT_ON_ERROR)
        exit(1);
}
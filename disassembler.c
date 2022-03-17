#include "disassembler.h"

#include <stdio.h>

//format and return with 0
#define FMT_RET(...) sprintf(dst,__VA_ARGS__); return 0;

//mnemonics stolen from https://github.com/craigthomas/Chip8Assembler :P
int disassemble(char dst[DISASSEMBLY_MAX_STR_LEN], uint16_t ins){
    uint16_t NNN =   (ins&0x0fff)>>0;
    uint8_t NN =    (ins&0x00ff)>>0;
    uint8_t N =     (ins&0x000f)>>0;
    uint8_t X =     (ins&0x0f00)>>8;
    uint8_t Y =     (ins&0x00f0)>>4;
    switch ((ins&0xf000)>>12) {
        case 0:
            switch (ins) {
                case 0x00E0: FMT_RET("CLR");
                case 0x00EE: FMT_RET("RTS");
                default: FMT_RET("SYS $%04x",NNN)
            }
            break;
        case 1: FMT_RET("JUMP $%04x",NNN);
        case 2: FMT_RET("CALL $%04x",NNN);
        case 3: FMT_RET("SKE v%01x,$%02x",X,NNN);
        case 4: FMT_RET("SKNE v%01x,$%02x",X,NNN);
        case 5: FMT_RET("SKRE v%01x,v%01x",X,Y);//TODO: verify last 4 bits are equal to 0
        case 6: FMT_RET("LOAD v%01x,$%02x", X,NN);
        case 7: FMT_RET("ADD v%01x,$%02x", X,NN);
        case 8:
            switch (N) {
                case 0: FMT_RET("MOVE v%01x,v%01x", X, Y);
                case 1: FMT_RET("OR v%01x,v%01x",X,Y);
                case 2: FMT_RET("AND v%01x,v%01x",X,Y);
                case 3: FMT_RET("XOR v%01x,v%01x",X,Y);
                case 4: FMT_RET("ADDR v%01x,v%01x",X,Y);
                case 5: FMT_RET("SUB v%01x,v%01x",X,Y);
                case 6: FMT_RET("SHR v%01x",X);
                case 7: FMT_RET("SUBR v%01x,v%01x",X,Y);
                case 0xe: FMT_RET("SHL v%01x",X);
            }
            break;
        case 9: FMT_RET("SKRNE v%01x,v%01x",X,Y);
        case 0xa: FMT_RET("LOADI $%04x",NNN);

        case 0xb: FMT_RET("JUMPI $%04x",NNN);//TODO: make this configurable
        case 0xc: FMT_RET("RAND v%01x,$%02x",X,NN);
        case 0xd: FMT_RET("DRAW $%01x,v%01x,v%01x",N,X,Y);

        case 0xe:
            switch (NN) {
                case 0x9e: FMT_RET("SKPR v%01x",X);
                case 0xa1: FMT_RET("SKUP v%01x",X);
            }
            break;
        
        case 0xf:
            switch (NN) {
                case 0x07: FMT_RET("MOVED v%01x",X);
                case 0x0a: FMT_RET("KEYD v%01x",X);
                case 0x15: FMT_RET("LOADD v%01x",X);
                case 0x18: FMT_RET("LOADS v%01x",X);
                case 0x1e: FMT_RET("ADDI v%01x",X);
                case 0x29: FMT_RET("LDSPR v%01x",X);
                case 0x33: FMT_RET("BCD v%01x",X);
                case 0x55: FMT_RET("STOR $%01x",X);
                case 0x65: FMT_RET("READ $%01x",X);
            }
            break;
    }
    sprintf(dst,"INVALID");
    return -1;
}
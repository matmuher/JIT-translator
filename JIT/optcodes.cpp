#include "JIT.h"

const int8_t    NOP         = 0x90,
                LEAVE       = 0xC9,
                RET         = 0xC3,
                PUSH_IMM    = 0x68,
                PUSH_REG    = 0x50,
                POP_REG     = 0x58;

const int16_t   POP_R8      = 0x5841,
                POP_R9      = 0x5941,
                PUSH_R8     = 0x5041,
                MOVABS_RSI  = 0xBE48,
                MOVABS_RCX  = 0xB948;

const int32_t   ADD_R8_R9   = 0x00C8014D,
                POP_EMPTY   = 0x08C48348,
                PUSH_RAM    = 0x000076FF,   // Push ram. Shift is to be in 3rd byte
                PUSH_REL    = 0x000071FF,   // Push from RAM relatively base register
                POP_RAM     = 0x0000468F,
                POP_REL     = 0x0000418F;


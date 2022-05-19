#include "JIT.h"

const int8_t    NOP         = 0x90,
                LEAVE       = 0xC9,
                RET         = 0xC3,
                PUSH_IMM    = 0x68,
                POP_OP1     = 0x58, // RAX
                POP_OP2     = 0x59, // RCX
                PUSH_OP1    = 0x50,
                JUMP_REL    = 0xE9,
                CALL        = 0xE8,
                POP_RDI     = 0x5F;



const int16_t   MOVABS_RSI  = 0xBE48,
                MOVABS_R11  = 0xBB49,
                PUSH_REG    = 0x5241,   // R11
                POP_REG     = 0x5A41,
                JG_REL      = 0x8F0F,
                JL_REL      = 0x8C0F,
                JE_REL      = 0x840F,
                JNE_REL     = 0x850F,
                JGE_REL     = 0x8D0F,
                JLE_REL     = 0x8E0F;

const int32_t   POP_EMPTY   = 0x08C48348,   // Are used to stack align
                PUSH_EMPTY   = 0x08EC8348,
                PUSH_RAM    = 0x000076FF,   // Push ram. Shift is to be in 3rd byte.
                POP_RAM     = 0x0000468F,
                PUSH_REL    = 0x0073FF41,   // Register-relative addressing push.
                POP_REL     = 0x00438F41,   // Shit is to be in 4th byte.
                SUB_OP12    = 0x00C82948,
                ADD_OP12    = 0x00C80148,
                IMUL_OP12   = 0x00E9F748,
                IDIV_OP12   = 0x00F9F748,
                ZERO_RDX    = 0x00D23148,
                CMP_OP12    = 0x00C83948;


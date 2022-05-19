#ifndef JIT_H_INCLUDED
#define JIT_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "../processor/processor.h"

struct jit
{
    int8_t* bin_buf;
    int32_t bin_buf_size;
    int8_t* buf_ptr;

    int64_t* ram_ptr;

    int32_t* src_bin;
    int32_t src_bin_size;
    int32_t src_ip;

    int8_t** cmd_equivalent;
};

jit* jit_init (int32_t bin_buf_size, const char* src_bin_path);
void jit_destr (jit* ma_jit);
void fill_with_nops (jit* ma_jit);
void put_ret (jit* ma_jit, int8_t bin_buf_shift);
void translate_src_bin (jit* ma_jit);
int64_t bin_execute (int8_t* array);

// Service functions
void print_bytes (int8_t* array, int32_t array_size, int32_t row_len);
void err (const char* error_msg, const char* error_function = NULL);

enum OPCODE_SIZE
{
    ONE_BYTE    = 1,
    TWO_BYTE    = 2,
    THREE_BYTE  = 3,
    FOUR_BYTE   = 4,
    FIVE_BYTE   = 5,
    TEN_BYTES   = 10,
};

// Operations codes

extern const int8_t NOP,
                    LEAVE,
                    RET,
                    PUSH_IMM,
                    POP_OP1,    // RAX
                    POP_OP2,    // RCX
                    PUSH_OP1,
                    JUMP_REL;

extern const int16_t    MOVABS_RSI,
                        MOVABS_R11,
                        PUSH_REG,   // R11
                        POP_REG;

extern const int32_t    POP_EMPTY,
                        PUSH_RAM,
                        POP_RAM,
                        PUSH_REL,
                        POP_REL,
                        SUB_OP12,
                        ADD_OP12,
                        IMUL_OP12,
                        IDIV_OP12,
                        ZERO_RDX;

#endif // JIT_H_INCLUDED

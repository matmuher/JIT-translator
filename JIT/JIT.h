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
};

jit* jit_init (int32_t bin_buf_size, const char* src_bin_path);
void jit_destr (jit* ma_jit);
void fill_with_nops (jit* ma_jit);
void put_ret (jit* ma_jit, int8_t bin_buf_shift);
void translate_src_bin (jit* ma_jit);
void bin_execute (int8_t* array);

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
                    PUSH_REG,
                    POP_REG;

extern const int16_t    POP_R8,
                        POP_R9,
                        PUSH_R8,
                        MOVABS_RSI,
                        MOVABS_RCX;

extern const int32_t    ADD_R8_R9,
                        POP_EMPTY,
                        PUSH_RAM,
                        PUSH_REL,
                        POP_RAM,
                        POP_REL;

#endif // JIT_H_INCLUDED

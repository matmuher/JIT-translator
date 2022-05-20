#ifndef JIT_H_INCLUDED
#define JIT_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <math.h>
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

    uint8_t is_stack_aligned;
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
    SIX_BYTE    = 6,
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
                    JUMP_REL,
                    CALL,
                    POP_RDI,
                    PUSH_RBP,
                    POP_RBP,

                    PUSH_RAX,
                    PUSH_RCX,
                    PUSH_RSI,
                    POP_RAX,
                    POP_RCX,
                    POP_RSI;

extern const int16_t    MOVABS_RSI,
                        MOVABS_R11,
                        PUSH_REG,   // R11
                        POP_REG,
                        JG_REL,
                        JL_REL,
                        JE_REL,
                        JNE_REL,
                        JGE_REL,
                        JLE_REL,
                        EXIT_CODE,
                        CQO,

                        PUSH_R10,
                        PUSH_R11,
                        PUSH_R12,
                        PUSH_R13,
                        POP_R10,
                        POP_R11,
                        POP_R12,
                        POP_R13;

extern const int32_t    POP_EMPTY,
                        PUSH_EMPTY,
                        PUSH_RAM,
                        POP_RAM,
                        PUSH_REL,
                        POP_REL,
                        SUB_OP12,
                        ADD_OP12,
                        IMUL_OP12,
                        IDIV_OP12,
                        ZERO_RDX,
                        CMP_OP12,
                        MOV_RBP_RSP;

#endif // JIT_H_INCLUDED

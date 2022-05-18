#ifndef JIT_H_INCLUDED
#define JIT_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

struct jit
{
    int8_t* bin_buf;
    int32_t bin_buf_size;
};

jit* jit_init (int32_t bin_buf_size);
void jit_dest (jit* ma_jit);
void fill_with_nops (jit* ma_jit);
void put_ret (jit* ma_jit, int8_t bin_buf_shift);

// Operations codes

extern const int8_t NOP_CODE,
                    LEAVE_CODE,
                    RET_CODE;


#endif // JIT_H_INCLUDED

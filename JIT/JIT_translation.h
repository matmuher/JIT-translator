#ifndef JIT_TRANSLATION_H_INCLUDED
#define JIT_TRANSLATION_H_INCLUDED

#include "JIT.h"

void translate_cmd (jit* ma_jit, int32_t cmd);

void translate_ariphmetic(jit* ma_jit, int32_t cmd);

void translate_memory_access(jit* ma_jit, int32_t cmd);
void push_registers (jit* ma_jit);
void pop_registers (jit* ma_jit);

void translate_jump(jit* ma_jit, int32_t cmd);
void translate_cond_jump (jit* ma_jit, int32_t cmd);

void translate_nop (jit* ma_jit);
void translate_hlt (jit* ma_jit);

enum func_params
    {
    HAS_ARG = 1,
    NO_ARG,
    HAS_RET,
    NO_RET,
    SAVE_REGS,
    NO_SAVE_REGS,
    };
void translate_function (jit* ma_jit, intptr_t func_ptr, func_params has_arg, func_params ret, func_params save_regs = SAVE_REGS);
void translate_call (jit* ma_jit, int32_t cmd);
void translate_ret (jit* ma_jit);
void translate_out (jit* ma_jit);
void translate_in (jit* ma_jit);
void translate_sqrt (jit* ma_jit);
void translate_say (jit* ma_jit, int32_t cmd);

void change_stack (jit* ma_jit, const int8_t* OPCODE, int8_t opcode_size);
void write_opcode(jit* ma_jit, const int8_t* opcode_ptr, int8_t opcode_size);
int8_t* ptr_8bit (const void* addr);

#endif // JIT_TRANSLATION_H_INCLUDED

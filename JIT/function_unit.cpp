#include "JIT_translation.h"

void translate_function (jit* ma_jit, intptr_t function_ptr, func_translate_arg has_arg, func_translate_arg ret)
{
    // Argument -> rdi
    if (has_arg == HAS_ARG) change_stack(ma_jit, ptr_8bit(&POP_RDI), ONE_BYTE);

    push_registers(ma_jit);

    if (!ma_jit->is_stack_aligned)
        write_opcode(ma_jit, ptr_8bit(&PUSH_EMPTY), FOUR_BYTE);

    int32_t shift = (intptr_t) function_ptr  - ((intptr_t) ma_jit->buf_ptr + FIVE_BYTE);
    int8_t func_call[FIVE_BYTE] = {CALL};

    *((int32_t*)(func_call + 1)) = shift;

    write_opcode(ma_jit, func_call, FIVE_BYTE);

    if (!ma_jit->is_stack_aligned)
        write_opcode(ma_jit, ptr_8bit(&POP_EMPTY), FOUR_BYTE);

    pop_registers(ma_jit);

    // Push input value in stack
    if (ret == HAS_RET) change_stack(ma_jit, ptr_8bit(&PUSH_OP1), ONE_BYTE);
}

void translate_out (jit* ma_jit)
    {
    extern int64_t out (int64_t x);
    translate_function(ma_jit, (intptr_t) out, HAS_ARG, HAS_RET);
    }

void translate_in (jit* ma_jit)
    {
    extern int64_t in (void);
    translate_function(ma_jit, (intptr_t) in, NO_ARG, HAS_RET);
    }

void translate_sqrt (jit* ma_jit)
    {
    extern int64_t int64_sqrt (int64_t value);
    translate_function(ma_jit, (intptr_t) int64_sqrt, HAS_ARG, HAS_RET);
    }

void translate_say (jit* ma_jit, int32_t cmd)
{
    extern void sayi (void);
    extern void sayn (void);

    switch (cmd)
    {
        case cmd_sayi:
            translate_function(ma_jit, (intptr_t) sayi, NO_ARG, NO_RET);
            break;
        case cmd_sayn:
            translate_function(ma_jit, (intptr_t) sayn, NO_ARG, NO_RET);
            break;
    }
}

void push_registers (jit* ma_jit)
{
    int16_t two_byte_regs[] = {PUSH_R10, PUSH_R11, PUSH_R12, PUSH_R13};
    int8_t regs_num = sizeof(two_byte_regs) / sizeof(two_byte_regs[0]);

    for (int8_t reg_id = 0; reg_id < regs_num; reg_id++)
        {
        change_stack(ma_jit, ptr_8bit(&two_byte_regs[reg_id]), TWO_BYTE);
        }

    int8_t one_byte_regs[] = {PUSH_RSI};
    regs_num = sizeof(one_byte_regs) / sizeof(one_byte_regs[0]);

    for (int8_t reg_id = 0; reg_id < regs_num; reg_id++)
        {;
         change_stack(ma_jit, ptr_8bit(one_byte_regs + reg_id), ONE_BYTE);
        }
}

void pop_registers (jit* ma_jit)
{
    int8_t one_byte_regs[] = {POP_RSI};
    int8_t regs_num = sizeof(one_byte_regs) / sizeof(one_byte_regs[0]);

    for (int8_t reg_id = regs_num - 1; reg_id >= 0; reg_id--)
    {;
        change_stack(ma_jit, ptr_8bit(one_byte_regs + reg_id), ONE_BYTE);
    }

    int16_t two_byte_regs[] = {POP_R10, POP_R11, POP_R12, POP_R13};
    regs_num = sizeof(two_byte_regs) / sizeof(two_byte_regs[0]);

    for (int8_t reg_id = regs_num - 1; reg_id >= 0; reg_id--)
        change_stack(ma_jit, ptr_8bit(two_byte_regs + reg_id), TWO_BYTE);
}

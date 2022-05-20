#include "JIT_translation.h"

void translate_jump (jit* ma_jit, int32_t cmd)
{
    // Jump argument is shifted in ma_proc architecture
    int32_t arg = ma_jit->src_bin[ma_jit->src_ip] + 1;
    int32_t shift = (intptr_t) ma_jit->cmd_equivalent[arg] - ((intptr_t) ma_jit->buf_ptr + FIVE_BYTE);

    int8_t jump_rel[FIVE_BYTE] = {JUMP_REL, 0};
    *((int32_t*) (jump_rel + 1)) = shift;

    write_opcode(ma_jit, ptr_8bit(jump_rel), FIVE_BYTE);

    ma_jit->src_ip += ONE_ARG;
}

void make_comparison (jit* ma_jit)
{
    change_stack(ma_jit, ptr_8bit(&POP_OP1), ONE_BYTE);
    change_stack(ma_jit, ptr_8bit(&POP_OP2), ONE_BYTE);
    write_opcode(ma_jit, ptr_8bit(&CMP_OP12), THREE_BYTE);
}

void translate_cond_jump (jit* ma_jit, int32_t cmd)
{
    make_comparison(ma_jit);

    // Argument is shift in source bin file
    int32_t arg = ma_jit->src_bin[ma_jit->src_ip] + 1;
    int32_t shift = (intptr_t) ma_jit->cmd_equivalent[arg] - ((intptr_t) ma_jit->buf_ptr + SIX_BYTE);

    int16_t jump_cond_rel[SIX_BYTE] = {};
    switch (cmd & CMD_MASK)
    {
        case cmd_ja:
            jump_cond_rel[0] = JG_REL;
            break;

        case cmd_jb:
            jump_cond_rel[0] = JL_REL;
            break;

        case cmd_je:
            jump_cond_rel[0] = JE_REL;
            break;

        case cmd_jne:
            jump_cond_rel[0] = JNE_REL;
            break;

        case cmd_jae:
            jump_cond_rel[0] = JGE_REL;
            break;

        case cmd_jbe:
            jump_cond_rel[0] = JLE_REL;
            break;
    }

    *((int32_t*) (jump_cond_rel + 1)) = shift;

    write_opcode(ma_jit, ptr_8bit(jump_cond_rel), SIX_BYTE);

    ma_jit->src_ip += ONE_ARG;
}


#include "JIT_translation.h"

void translate_arithmetic(jit* ma_jit, int32_t cmd)
{
    change_stack(ma_jit, ptr_8bit(&POP_OP1), ONE_BYTE);
    change_stack(ma_jit, ptr_8bit(&POP_OP2), ONE_BYTE);

    switch(cmd)
    {
        case(cmd_add):
            write_opcode(ma_jit, ptr_8bit(&ADD_OP12), THREE_BYTE);
            break;
        case(cmd_sub):
            write_opcode(ma_jit, ptr_8bit(&SUB_OP12), THREE_BYTE);
            break;
        case(cmd_mlt):
            write_opcode(ma_jit, ptr_8bit(&CQO), TWO_BYTE);
            write_opcode(ma_jit, ptr_8bit(&IMUL_OP12), THREE_BYTE);
            break;
        case(cmd_saw):
            write_opcode(ma_jit, ptr_8bit(&CQO), TWO_BYTE);
            write_opcode(ma_jit, ptr_8bit(&IDIV_OP12), THREE_BYTE);
            break;
        default:
            err("Unknown ariphmetic");
    }

    change_stack(ma_jit, ptr_8bit(&PUSH_OP1), ONE_BYTE);
    ma_jit->src_ip += ZERO_ARGS;
}

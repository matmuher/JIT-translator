#include "JIT_translation.h"

void translate_nop (jit* ma_jit)
{
    write_opcode(ma_jit, ptr_8bit(&NOP), ONE_BYTE);
}

void exit_code (jit* ma_jit)
{
    write_opcode(ma_jit, ptr_8bit(&EXIT_CODE), TWO_BYTE);
}

void translate_hlt(jit* ma_jit)
{
    exit_code(ma_jit);

    change_stack(ma_jit, ptr_8bit(&POP_RBP), ONE_BYTE);
    // Return to bin_execute() function
    write_opcode(ma_jit, ptr_8bit(&RET), ONE_BYTE);
}

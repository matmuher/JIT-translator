#include "JIT_translation.h"

void translate_memory_access(jit* ma_jit, int32_t cmd)
{
    extern void translate_push (jit* ma_jit, int32_t cmd, int32_t arg);
    extern void translate_pop  (jit* ma_jit, int32_t cmd, int32_t arg);

    int32_t arg  = ma_jit->src_bin[ma_jit->src_ip];

    switch (cmd & CMD_MASK)
    {
        case(cmd_push):
            translate_push(ma_jit, cmd, arg);
            break;

        case(cmd_pop):
            translate_pop(ma_jit, cmd, arg);
            break;
    }

    ma_jit->src_ip += ONE_ARG;
}

void translate_push (jit* ma_jit, int32_t cmd, int32_t arg)
{
    switch (cmd & OP_TYPE_MASK)
    {
        case(REG_REL_MASK):
            {
                // Put shift in the 4th byte
                uint32_t reg_rel_push = PUSH_REL | (arg << THREE_BYTE * 8);
                change_stack(ma_jit, ptr_8bit(&reg_rel_push), FOUR_BYTE);
                break;
            }
        case(REG_MASK):
            {
                uint16_t push_reg = PUSH_REG + (arg << ONE_BYTE * 8);
                change_stack(ma_jit, ptr_8bit(&push_reg), TWO_BYTE);
                break;
            }

        case(IMM_MASK):
            {
                // Put opcode in the 1st byte
                uint64_t imm_push = PUSH_IMM | (arg << ONE_BYTE * 8);
                change_stack(ma_jit, ptr_8bit(&imm_push), FIVE_BYTE);
                break;
            }

        case(RAM_MASK):
            {
                uint32_t ram_push = PUSH_RAM | (arg << TWO_BYTE * 8);
                change_stack(ma_jit, ptr_8bit(&ram_push), THREE_BYTE);
                break;
            }
    }
}

void translate_pop (jit* ma_jit, int32_t cmd, int32_t arg)
{
    switch (cmd & OP_TYPE_MASK)
    {
        case(REG_REL_MASK):
            {
                uint32_t reg_rel_pop = POP_REL | (arg << THREE_BYTE * 8);
                change_stack(ma_jit, ptr_8bit(&reg_rel_pop), FOUR_BYTE);
                break;
            }
        case(REG_MASK):
            {
                uint16_t pop_reg = POP_REG + (arg << ONE_BYTE * 8);
                change_stack(ma_jit, ptr_8bit(&pop_reg), TWO_BYTE);
                break;
            }
        case(IMM_MASK):
                change_stack(ma_jit, ptr_8bit(&POP_EMPTY), FOUR_BYTE);
                break;

        case(RAM_MASK):
            {
                uint32_t ram_pop = POP_RAM | (arg << TWO_BYTE * 8);
                change_stack(ma_jit, ptr_8bit(&ram_pop), THREE_BYTE);
                break;
            }
    }
}

#include "JIT.h"

void write_opcode(jit* ma_jit, const int8_t* opcode_ptr, int8_t opcode_size);
int8_t* ptr_8bit (const void* addr);
void translate_ariphmetic(jit* ma_jit, int32_t cmd);
void translate_memory_access(jit* ma_jit, int32_t cmd);
void translate_hlt(jit* ma_jit, int32_t cmd);

void fill_with_nops (jit* ma_jit)
{
    for (int32_t byte_id = 0; byte_id < ma_jit->bin_buf_size; byte_id++)
        ma_jit->bin_buf[byte_id] = NOP;
}

void ram_init (jit* ma_jit)
{
    int8_t movabs_opdcode[TEN_BYTES] = {};
    int8_t* opcode_ptr = movabs_opdcode;

    // Set register for absolute addressing in RAM
    *((int16_t*) opcode_ptr) = MOVABS_RSI;
    opcode_ptr += 2;
    *((int64_t*) opcode_ptr) = (uintptr_t) ma_jit->ram_ptr;
    write_opcode(ma_jit, ptr_8bit(movabs_opdcode), TEN_BYTES);

    // Set register for relative addressing in RAM
    opcode_ptr -= 2;
    *((int16_t*) opcode_ptr) = MOVABS_RCX;
    write_opcode(ma_jit, ptr_8bit(movabs_opdcode), TEN_BYTES);
}

extern void translate_cmd (jit* ma_jit, int32_t cmd);

void translate_src_bin (jit* ma_jit)
{
    ram_init(ma_jit);

    while ((ma_jit->src_ip) < (ma_jit->src_bin_size))
        translate_cmd(ma_jit, ma_jit->src_bin[ma_jit->src_ip++]);
}

void translate_cmd (jit* ma_jit, int32_t cmd)
{
    int32_t pure_cmd = cmd & CMD_MASK;

    const int8_t PURE_CMD_LEN = 3;
    char cmd_hex[PURE_CMD_LEN] = {};
    sprintf (cmd_hex, "%x", pure_cmd);

    switch (pure_cmd)
    {
        case cmd_add:
        case cmd_sub:
        case cmd_mlt:
        case cmd_saw:
            translate_ariphmetic(ma_jit, cmd);
            break;

        case cmd_push:
        case cmd_pop:
            translate_memory_access(ma_jit, cmd);
            break;

        case cmd_hlt:
            translate_hlt(ma_jit, cmd);
            break;

        #if 0
        case cmd_jump:
        case cmd_ja:
        case cmd_jb:
        case cmd_je:
        case cmd_jne:
        case cmd_jae:
        case cmd_jbe:
        case cmd_call:
        case cmd_ret:
            translate_jump(cmd);
            break;

        case cmd_say:
            translate_nop(cmd);
            break;

        case cmd_sqrt:
            translate_sqrt(cmd);
            break;

        case cmd_out:
        case cmd_in:
            translate_ui(cmd); // ui = user interaction
        #endif

        default: err("Unrecognized cmd", cmd_hex);
    }
}

void write_opcode(jit* ma_jit, const int8_t* opcode_ptr, int8_t opcode_size)
{
    const int8_t* opcode_end = opcode_ptr + opcode_size;
    while (opcode_ptr < opcode_end)
        *(ma_jit->buf_ptr++) = *(opcode_ptr++);
}

void translate_ariphmetic(jit* ma_jit, int32_t cmd)
{
    write_opcode(ma_jit, ptr_8bit(&POP_R8), TWO_BYTE);
    write_opcode(ma_jit, ptr_8bit(&POP_R9), TWO_BYTE);

    switch(cmd)
    {
        case(cmd_add):
            write_opcode(ma_jit, ptr_8bit(&ADD_R8_R9), THREE_BYTE);
            break;
        case(cmd_sub):
        case(cmd_mlt):
        case(cmd_saw):
        default:
            err("Unknown ariphmetic");
    }

    write_opcode(ma_jit, ptr_8bit(&PUSH_R8), TWO_BYTE);
    ma_jit->src_ip += ZERO_ARGS;
}

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
                uint32_t reg_rel_push = PUSH_REL | (arg << TWO_BYTE * 8);
                write_opcode(ma_jit, ptr_8bit(&reg_rel_push), THREE_BYTE);
                break;
            }
        case(REG_MASK):
            {
                uint8_t push_reg = PUSH_REG + arg;
                write_opcode(ma_jit, ptr_8bit(&push_reg), ONE_BYTE);
                break;
            }

        case(IMM_MASK):
            {
                // Put opcode in the begging
                uint64_t imm_push = PUSH_IMM | (arg << ONE_BYTE * 8);
                write_opcode(ma_jit, ptr_8bit(&imm_push), FIVE_BYTE);
                break;
            }

        case(RAM_MASK):
            {
                uint32_t ram_push = PUSH_RAM | (arg << TWO_BYTE * 8);
                write_opcode(ma_jit, ptr_8bit(&ram_push), THREE_BYTE);
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
                uint32_t reg_rel_push = POP_REL | (arg << TWO_BYTE * 8);
                write_opcode(ma_jit, ptr_8bit(&reg_rel_push), THREE_BYTE);
                break;
            }
        case(REG_MASK):
            {
                uint8_t pop_reg = POP_REG + arg;
                write_opcode(ma_jit, ptr_8bit(&pop_reg), ONE_BYTE);
                break;
            }
        case(IMM_MASK):
                write_opcode(ma_jit, ptr_8bit(&POP_EMPTY), FOUR_BYTE);
                break;

        case(RAM_MASK):
            {
                uint32_t ram_pop = POP_RAM | (arg << TWO_BYTE * 8);
                write_opcode(ma_jit, ptr_8bit(&ram_pop), THREE_BYTE);
                break;
            }
    }
}

void translate_hlt(jit* ma_jit, int32_t cmd)
{
    // Return to bin_execute function
    write_opcode(ma_jit, ptr_8bit(&RET), ONE_BYTE);
}

int8_t* ptr_8bit (const void* addr)
{
    return (int8_t*) addr;
}

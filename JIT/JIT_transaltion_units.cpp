#include "JIT_translation.h"

void ram_init (jit* ma_jit);
void enter (jit* ma_jit);

void translate_src_bin (jit* ma_jit)
{
    for (int8_t run_id = 0; run_id < 2; run_id++)
    {
        enter(ma_jit);

        ram_init(ma_jit);

        while ((ma_jit->src_ip) < (ma_jit->src_bin_size))
        {
            ma_jit->cmd_equivalent[ma_jit->src_ip] = ma_jit->buf_ptr;
            translate_cmd(ma_jit, ma_jit->src_bin[ma_jit->src_ip++]);
        }

        ma_jit->buf_ptr = ma_jit->bin_buf;
        ma_jit->src_ip = 0;
        ma_jit->is_stack_aligned = 0;
    }
}

void translate_cmd (jit* ma_jit, int32_t cmd)
{
    int32_t pure_cmd = cmd & CMD_MASK;

    const int8_t PURE_CMD_LEN = 3;
    char cmd_dec[PURE_CMD_LEN] = {};
    sprintf (cmd_dec, "%d", pure_cmd);

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
            translate_hlt(ma_jit);
            break;

        case cmd_jump:
            translate_jump(ma_jit, cmd);
            break;

        case cmd_ja:
        case cmd_jb:
        case cmd_je:
        case cmd_jne:
        case cmd_jae:
        case cmd_jbe:
            translate_cond_jump(ma_jit, cmd);
            break;

        case cmd_call:
            translate_call(ma_jit, cmd);
            break;

        case cmd_ret:
            translate_ret(ma_jit);
            break;

        case cmd_out:
            translate_out(ma_jit);
            break;

        case cmd_in:
            translate_in(ma_jit);
            break;

        case cmd_sqrt:
            translate_sqrt(ma_jit);
            break;

        case cmd_sayi:
        case cmd_sayn:
        case cmd_say:
            translate_say(ma_jit, cmd);
            break;

        default: err("Unrecognized cmd", cmd_dec);
    }
}

void change_stack (jit* ma_jit, const int8_t* OPCODE, int8_t opcode_size)
{
    ma_jit->is_stack_aligned = (ma_jit->is_stack_aligned + 1) & 1;
    write_opcode(ma_jit, OPCODE, opcode_size);
}

void enter (jit* ma_jit)
{
    change_stack(ma_jit, ptr_8bit(&PUSH_RBP), ONE_BYTE);
    write_opcode(ma_jit, ptr_8bit(&MOV_RBP_RSP), THREE_BYTE);
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
    *((int16_t*) opcode_ptr) = MOVABS_R11;
    write_opcode(ma_jit, ptr_8bit(movabs_opdcode), TEN_BYTES);
}

void write_opcode(jit* ma_jit, const int8_t* opcode_ptr, int8_t opcode_size)
{
    const int8_t* opcode_end = opcode_ptr + opcode_size;
    while (opcode_ptr < opcode_end)
        *(ma_jit->buf_ptr++) = *(opcode_ptr++);
}

int8_t* ptr_8bit (const void* addr)
{
    return (int8_t*) addr;
}


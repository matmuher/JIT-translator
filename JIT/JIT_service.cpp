#include "JIT.h"

void err (const char* error_msg, const char* error_function)
{
    if (error_function)
        printf("[ERROR:%s]: %s\n", error_function, error_msg);
    else
        printf("[ERROR]: %s\n", error_msg);
}

void read_src_bin (jit* ma_jit, const char* src_bin_path)
{
    FILE* src_bin_file = fopen(src_bin_path, "r");

    if (!src_bin_file)
        err("Can't open source binary");

    // Read service structure
    bin_info src_bin_info = {0, 0, 0};
    if (!fread (&src_bin_info, sizeof (char), sizeof (src_bin_info), src_bin_file))
        err("Can't read service structure");

    // Verify service structure
    assert (src_bin_info.signature == MY_SIGN);
    assert (src_bin_info.version == version);

    // Read ma_proc binary
    int32_t* src_bin = (int32_t*) calloc (src_bin_info.bin_size, sizeof (int32_t));
    if (!fread (src_bin, sizeof (int32_t), src_bin_info.bin_size, src_bin_file))
        err("Can't read ma_binary");

    ma_jit->src_bin = src_bin;
    ma_jit->src_bin_size = src_bin_info.bin_size;;

    fclose(src_bin_file);
}

void print_bytes (int8_t* array, int32_t array_size, int32_t row_len)
{
    for (int32_t byte_id = 0; byte_id < array_size; byte_id++)
    {
        printf("%x ", array[byte_id]);
        if (byte_id && byte_id % row_len == 0)
            putchar('\n');
    }
}

void print_jit (jit* ma_jit)
{
      printf (
                "bin_buf: %p\n"
                "bin_buf_size: %d\n"
                "buf_ptr: %p\n"
                "ram_ptr: %p\n"
                "src_bin: %p\n"
                "src_bin_size: %d\n"
                "src_ip: %d\n"
                "cmd_equivalent: %p\n"
                "is_stack_aligned: %d\n\n",
                ma_jit->bin_buf,
                ma_jit->bin_buf_size,
                ma_jit->buf_ptr,
                ma_jit->ram_ptr,
                ma_jit->src_bin,
                ma_jit->src_bin_size,
                ma_jit->src_ip,
                ma_jit->cmd_equivalent,
                ma_jit->is_stack_aligned);
}

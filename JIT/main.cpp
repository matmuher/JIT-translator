#include "JIT.h"

int main ()
{
    const int32_t BIN_BUF_SIZE = 50;

    jit* ma_jit = jit_init(BIN_BUF_SIZE, "../binary_my_binary");

    fill_with_nops(ma_jit);
    //put_ret(ma_jit, ma_jit->bin_buf_size);
    translate_src_bin(ma_jit);

    // asm ("push rax\n\t");
    int64_t return_value = -1;
    return_value = bin_execute(ma_jit->bin_buf);
    printf("JIT returned %u\n", return_value);
    // asm ("pop rax\n\t");
    jit_destr(ma_jit);

    return 0;
}

#include "JIT.h"

int main ()
{
    const int32_t BIN_BUF_SIZE = 50;

    jit* ma_jit = jit_init(BIN_BUF_SIZE, "../binary_my_binary");

    fill_with_nops(ma_jit);
    translate_src_bin(ma_jit);

    int64_t return_value = -1;
    return_value = bin_execute(ma_jit->bin_buf);
    printf("JIT returned %lu\n", return_value);
    jit_destr(ma_jit);

    return 0;
}

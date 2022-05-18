#include "JIT.h"

typedef void (*phony_function)(void);

void bin_execute (int8_t* array)
{
    phony_function ma_func = (phony_function) array;
    ma_func();
}

int main ()
{
    // FILE* ma_proc_bin = fopen("../binary_my_binary", "r");
    // fclose(ma_proc_bin);

    const int32_t BIN_BUF_SIZE = 10;

    jit* ma_jit = jit_init(BIN_BUF_SIZE);
    fill_with_nops(ma_jit);
    put_ret(ma_jit, ma_jit->bin_buf_size);

    bin_execute(ma_jit->bin_buf);

    return 0;
}

#include "JIT.h"

int32_t out (int32_t x)
{
    printf("%d\n", x);

    return x;
}

int32_t in (void)
{
    int user_input = 0;
    scanf("%d", &user_input);

    return user_input;
}

int32_t int32_sqrt (int32_t value)
{
    return lround(sqrt((double) value));
}

int main ()
{
    #if 1
    const int32_t BIN_BUF_SIZE = 50;

    system("cd .. && ./ma_asm asm_scripts/sum.txt");

    jit* ma_jit = jit_init(BIN_BUF_SIZE, "../binary_my_binary");

    fill_with_nops(ma_jit);
    translate_src_bin(ma_jit);

    int64_t return_value = -1;
    return_value = bin_execute(ma_jit->bin_buf);
    printf("JIT returned %lu\n", return_value);
    jit_destr(ma_jit);
    #endif

    return 0;
}

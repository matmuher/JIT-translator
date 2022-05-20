#include "JIT.h"

void sayi (void)
{
    puts("Infinity roots");
}

void sayn (void)
{
    puts("No roots");
}

int64_t out (int64_t x)
{
    printf("%d\n", x);

    return x;
}

int64_t in (void)
{
    int user_input = 0;
    scanf("%d", &user_input);

    return user_input;
}

int64_t int64_sqrt (int64_t value)
{
    return llround(sqrt((double) value));
}

int main ()
{
    #if 1
    const int32_t BIN_BUF_SIZE = 1000;

    // system("cd .. && ./ma_asm asm_scripts/square_code.txt");

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

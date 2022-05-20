#include "JIT.h"
#include <string.h>

int main (int, char* argv[])
{
    #if 0
    char* file_name = argv[1] ? argv[1] : (char*) "fib_code.txt";

    const int32_t SCRIPT_PATH_LEN = 100;
    char script_path[SCRIPT_PATH_LEN] = "cd .. && ./ma_asm asm_scripts/";
    puts(script_path);
    strcat(script_path, file_name);
    system(script_path);
    #endif

    const int32_t BIN_BUF_SIZE = 1000;
    jit* ma_jit = jit_init(BIN_BUF_SIZE, "../binary_my_binary");
    translate_src_bin(ma_jit);

    int64_t return_value = -1;
    return_value = bin_execute(ma_jit->bin_buf);

    // printf("JIT returned %lu\n", return_value);

    jit_destr(ma_jit);

    return 0;
}

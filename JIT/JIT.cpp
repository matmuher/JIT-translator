#include "JIT.h"

extern void read_src_bin (jit* ma_jit, const char* src_bin_path);
extern void fill_with_nops (jit* ma_jit);

jit* jit_init (int32_t bin_buf_size, const char* src_bin_path)
{
    const int8_t QWORD = 8; // 8 bytes
    const int16_t PAGE_SIZE = 4096;

    jit* ma_jit = (jit*) calloc (1, sizeof(jit));

    ma_jit->bin_buf_size = bin_buf_size;

    bin_buf_size += RAM_SIZE * QWORD;

    // Allocate buffer for execution and for RAM
    if (posix_memalign ((void**) &ma_jit->bin_buf, PAGE_SIZE, sizeof(int8_t) * bin_buf_size))
        err("posix_memalign can't allocate aligned memory");
    if(mprotect(ma_jit->bin_buf, bin_buf_size, PROT_EXEC | PROT_WRITE | PROT_READ))
        err("mprotect can't give needed rights");
    ma_jit->buf_ptr = ma_jit->bin_buf;
    ma_jit->ram_ptr = (int64_t*)(ma_jit->bin_buf + bin_buf_size);

    fill_with_nops(ma_jit);

    // Load source binary
    read_src_bin(ma_jit, src_bin_path);
    ma_jit->src_ip = 0;

    return ma_jit;
}

void jit_destr (jit* ma_jit)
{
    free(ma_jit->bin_buf);
    free(ma_jit->src_bin);
    free(ma_jit);
}

typedef void (*phony_function)(void);

void bin_execute (int8_t* array)
{
    phony_function ma_func = (phony_function) array;
    ma_func();
}


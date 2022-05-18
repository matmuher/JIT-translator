#include "JIT.h"

jit* jit_init (int32_t bin_buf_size)
{
    const int16_t PAGE_SIZE = 4096;

    jit* ma_jit = (jit*) calloc (1, sizeof(jit));

    posix_memalign ((void**) &ma_jit->bin_buf, PAGE_SIZE, sizeof(int8_t) * bin_buf_size);
    mprotect(ma_jit->bin_buf, bin_buf_size, PROT_EXEC | PROT_WRITE | PROT_READ);

    ma_jit->bin_buf_size = bin_buf_size;

    return ma_jit;
}

void jit_dest (jit* ma_jit)
{
    free(ma_jit->bin_buf);
    free(ma_jit);
}

void fill_with_nops (jit* ma_jit)
{
    for (int32_t byte_id = 0; byte_id < ma_jit->bin_buf_size; byte_id++)
        ma_jit->bin_buf[byte_id] = NOP_CODE;
}

void put_ret (jit* ma_jit, int8_t bin_buf_shift)
{
    ma_jit->bin_buf[bin_buf_shift - 1] = RET_CODE;
}

#include "JIT.h"

void sayi (void)
{
    puts("Infinity roots");
}

void sayn (void)
{
    puts("No roots");
}

void say (void)
{
    puts("[Meow]");
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


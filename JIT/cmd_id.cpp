#include "JIT.h"

#define DEF_CMD(cmd_name, cmd_id)\
    const int8_t cmd_name = cmd_id;
#include "../processor/cmd.h"

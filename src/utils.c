#include "utils.h"
#include <stdint.h>

int is_big_endian() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}

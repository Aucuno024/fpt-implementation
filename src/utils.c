#include "utils.h"
#include "csapp.h"
#include <stdint.h>

int get_endianess() {
    static uint32_t one = 1;
    return ((* (uint8_t *) &one) == 0);
}

int open_file_r(char path[], int *fd)
{
    return (*fd = open(path, O_RDONLY, 0)) != -1;
}


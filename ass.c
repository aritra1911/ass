#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "ass.h"

int
hex32el(char **hex, uint32_t data, size_t spaces)
{
    const size_t BITS = 32;
    size_t bytes = BITS / 8;
    size_t buflen = bytes*2 + spaces*(bytes-1) + 1;
    size_t offset = 0;

    if ( !*hex ) {
        if ( !(*hex = malloc(buflen * sizeof(char))) )
            return UR_MOM;  /* She occupied the entire memory */
        /* Let the caller deal with the free */
    }

    for (size_t _=0; _<bytes-1; _++) {
        sprintf(*hex + offset, "%02x", data & 0xff);
        offset += 2;
        memset(*hex + offset, ' ', spaces);
        offset += spaces;
        data >>= 8;
    }
    sprintf(*hex + offset, "%02x", data & 0xff);
    offset += 2;
    (*hex)[offset] = '\0';
}

#include <stdio.h>
#include <stdlib.h>

#include "ass.h"

int
main(void)
{
    printf("Let's try to decode a simple ADD instruction :\n\n"

           "    ADD Rs, PC, #4\n\n");

    parse_instruction("    ADD       Rs,   PC,    #4       ");

    return EXIT_SUCCESS;
}

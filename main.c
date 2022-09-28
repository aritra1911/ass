#include <stdio.h>
#include <stdlib.h>

#include "ass.h"

int
main(void)
{
    printf("Let's try to parse simple ADD instructions :\n\n");

    parse_instruction(" ADD     R2, PC, #4  ");
    parse_instruction(" ADDS    R5, R6, #5  ");
    parse_instruction(" ADDEQS  R1, PC, #C  ");

    return EXIT_SUCCESS;
}

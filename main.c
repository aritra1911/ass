#include <stdio.h>
#include <stdlib.h>

#include "ass.h"

int
main(void)
{
    printf("Let's try to parse simple ADD instructions :\n\n");

    /*                  OpCode  Rd      Rn      shifter_op */
    parse_instruction(" ADD     R2,     PC,     #4  ");
    parse_instruction(" ADDS    R5,     R6,     #5  ");
    parse_instruction(" ADDEQS  R1,     PC,     #C  ");
    parse_instruction(" ADDNEQS PC,     R10,    #2  "); /* WARN: invalid cond */
    parse_instruction(" ADDNEQ  R15,    R13,    #B  "); /* WARN: invalid cond */
    parse_instruction(" ADDNES  PC,     R10,    #2  ");
    parse_instruction(" ADDNE   R15,    R13,    #B  ");

    return EXIT_SUCCESS;
}

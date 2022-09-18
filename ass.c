#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "strtrim.h"
#include "ass.h"

int
parse_instruction(const char *cinstr)
{
    /* Let's try to parse a simple ADD inst. It kinda looks like this:
     *
     *   31  28 27 26 25  24     21 20  19  16 15  12 11              0
     *  +--------------------------------------------------------------+
     *  | cond | 0 0 | 1 | 0 1 0 0 | S |  Rn  |  Rd  | shifter operand |
     *  +--------------------------------------------------------------+
     *
     *  Syntax:
     *
     *   ADD{cond}{S}   Rd, Rn, <shifter_operand>
     *
     */

    /* Some hardcoded values until the parser gets better */
    uint32_t cond = 0xe;    /* 1110 : AL */
    uint32_t s = 0;         /* don't affect status flags */

    /* Let's make a copy of `cinstr` which we can then modify */
    char minstr[256];
    strcpy(minstr, cinstr);

    char *instr = strtrim(minstr);

    char *opcode = strtok(instr, ", ");
    char *rd = strtok(NULL, ", ");
    char *rn = strtok(NULL, ", ");
    char *shifter_operand = strtok(NULL, ", ");

    printf("           OpCode : '%s'\n", opcode);
    printf("               Rd : '%s'\n", rd);
    printf("               Rn : '%s'\n", rn);
    printf("  shifter operand : '%s'\n", shifter_operand);

    return 0;
}

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "strtrim.h"
#include "ass.h"

#define OPCODE_LEN  3
#define COND_LEN    2

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

    /* Let's make a copy of `cinstr` which we can then modify */
    char minstr[256];
    strcpy(minstr, cinstr);

    char *instr = strtrim(minstr);

    if ( isatty(STDOUT_FILENO) )
        printf("\x1b[32m>\x1b[0m \x1b[31m%s\x1b[0m\n", instr);
    else
        printf("> %s\n", instr);

    char *opcode = strtok(instr, " ");
    char *rd = strtok(NULL, ", ");
    char *rn = strtok(NULL, ", ");
    char *shifter_operand = strtok(NULL, ", "); /* TODO: Fix this */

    instruct_t instruct;
    char *op = opcode;

    /* Parse opcode */
         if ( !strncmp(op, "AND", OPCODE_LEN) ) instruct.opcode = AND;
    else if ( !strncmp(op, "EOR", OPCODE_LEN) ) instruct.opcode = EOR;
    else if ( !strncmp(op, "SUB", OPCODE_LEN) ) instruct.opcode = SUB;
    else if ( !strncmp(op, "RSB", OPCODE_LEN) ) instruct.opcode = RSB;
    else if ( !strncmp(op, "ADD", OPCODE_LEN) ) instruct.opcode = ADD;
    else if ( !strncmp(op, "ADC", OPCODE_LEN) ) instruct.opcode = ADC;
    else if ( !strncmp(op, "SBC", OPCODE_LEN) ) instruct.opcode = SBC;
    else if ( !strncmp(op, "RSC", OPCODE_LEN) ) instruct.opcode = RSC;
    else if ( !strncmp(op, "TST", OPCODE_LEN) ) instruct.opcode = TST;
    else if ( !strncmp(op, "TEQ", OPCODE_LEN) ) instruct.opcode = ADD;
    else if ( !strncmp(op, "CMP", OPCODE_LEN) ) instruct.opcode = CMP;
    else if ( !strncmp(op, "CMN", OPCODE_LEN) ) instruct.opcode = CMN;
    else if ( !strncmp(op, "ORR", OPCODE_LEN) ) instruct.opcode = ORR;
    else if ( !strncmp(op, "MOV", OPCODE_LEN) ) instruct.opcode = MOV;
    else if ( !strncmp(op, "BIC", OPCODE_LEN) ) instruct.opcode = BIC;
    else if ( !strncmp(op, "MVN", OPCODE_LEN) ) instruct.opcode = MVN;
    else {
        fprintf(stderr, "Unknown / Unimplemented instruction : '%s'\n", op);
        return -1;
    }

    /*
     * Go beyond the instruction opcode to check if we got any
     * condition or status affect flag.
     */
    op += OPCODE_LEN;

    /* Parse Status bit *
    if ( opcode[strlen(opcode) - 1] == 'S' ) {
        instruct.s = 1;
        opcode[strlen(opcode) - 1] = '\0';
    }
    */

    /* Parse Condition (if any) */
    instruct.cond = AL; /* default when not specified */
    if ( strlen(op) >= 2 ) {
             if ( !strncmp(op, "EQ", COND_LEN) )   instruct.cond = EQ;
        else if ( !strncmp(op, "NE", COND_LEN) )   instruct.cond = NE;
        else if ( !strncmp(op, "CS", COND_LEN) ||
                  !strncmp(op, "HS", COND_LEN) )   instruct.cond = CS;
        else if ( !strncmp(op, "CC", COND_LEN) ||
                  !strncmp(op, "LO", COND_LEN) )   instruct.cond = CC;
        else if ( !strncmp(op, "MI", COND_LEN) )   instruct.cond = MI;
        else if ( !strncmp(op, "PL", COND_LEN) )   instruct.cond = PL;
        else if ( !strncmp(op, "VS", COND_LEN) )   instruct.cond = VS;
        else if ( !strncmp(op, "VC", COND_LEN) )   instruct.cond = VC;
        else if ( !strncmp(op, "HI", COND_LEN) )   instruct.cond = HI;
        else if ( !strncmp(op, "LS", COND_LEN) )   instruct.cond = LS;
        else if ( !strncmp(op, "GE", COND_LEN) )   instruct.cond = GE;
        else if ( !strncmp(op, "LT", COND_LEN) )   instruct.cond = LT;
        else if ( !strncmp(op, "GT", COND_LEN) )   instruct.cond = GT;
        else if ( !strncmp(op, "LE", COND_LEN) )   instruct.cond = LE;
        else if ( !strncmp(op, "AL", COND_LEN) )   instruct.cond = AL;
        else if ( !strncmp(op, "NV", COND_LEN) )   instruct.cond = NV;

        op += COND_LEN;
    }

    instruct.s = *op == 'S';

    printf("           OpCode : '%s' (0x%x)\n", opcode, instruct.opcode);
    printf("               Rd : '%s'\n", rd);
    printf("               Rn : '%s'\n", rn);
    printf("  shifter operand : '%s'\n", shifter_operand);
    printf("             cond : 0x%x\n", instruct.cond);
    printf("                s : %u\n\n", instruct.s);

    return 0;
}

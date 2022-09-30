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

    /* Immediate operand value */
    parse_instruction(" MOV     R0,     #0              ; Move zero to R0   ");
    parse_instruction(" ADD     R3,     R3,     #1      ; Add one to the value of register 3    ");
    parse_instruction(" CMP     R7,     #1000           ; Compare value of R7 with 1000 ");
    parse_instruction(" BIC     R9,     R8,     #0xFF00 ; Clear bits 8-15 of R8 and store in R9 ");

    /* Register operand value */
    parse_instruction(" MOV     R2,     R0          ; Move the value of R0 to R2    ");
    parse_instruction(" ADD     R4,     R3,     R2  ; Add R2 to R3, store result in R4  ");
    parse_instruction(" CMP     R7,     R8          ; Add R2 to R3, store result in R4  ");

    /* Shifted register operand value */
    parse_instruction(" MOV     R2,     R0,     #2              ; Shift R0 left by 2, write to R2, (R2=R0x4)    ");
    parse_instruction(" ADD     R9,     R5,     R5,     LSL #3  ; R9 = R5 + R5 x 8 or R9 = R5 x 9   ");
    parse_instruction(" RSB     R9,     R5,     R5,     LSL #3  ; R9 = R5 x 8 - R5 or R9 = R5 x 7   ");
    parse_instruction(" SUB     R10,    R9,     R8,     LSR #4  ; R10 = R9 - R8 / 16    ");
    parse_instruction(" MOV     R12,    R4,     ROR     R3      ; R12 = R4 rotated right by value of R3 ");

    return EXIT_SUCCESS;
}

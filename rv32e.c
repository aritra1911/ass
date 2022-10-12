#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "strtrim.h"
#include "ass.h"

/*
 * Quoting From page 33 of the `riscv-spec-20191213.pdf`:
 *
 *  This chapter describes a draft proposal for the RV32E base integer instruction set, which is a
 *  reduced version of RV32I designed for embedded systems. The only change is to reduce the number
 *  of integer registers to 16. This chapter only outlines the differences between RV32E and RV32I,
 *  and so should be read after Chapter 2.
 *
 *      RV32E was designed to provide an even smaller base core for embedded microcontrollers. Al-
 *      though we had mentioned this possibility in version 2.0 of this document, we initially resisted
 *      defining this subset. However, given the demand for the smallest possible 32-bit microcontroller,
 *      and in the interests of preempting fragmentation in this space, we have now defined RV32E as
 *      a fourth standard base ISA in addition to RV32I, RV64I, and RV128I. There is also interest in
 *      defining an RV64E to reduce context state for highly threaded 64-bit processors.
 *
 *  RV32E reduces the integer register count to 16 general-purpose registers, (x0–x15), where x0 is a
 *  dedicated zero register.
 */

typedef enum {
    x0,  x1,  x2,  x3,
    x4,  x5,  x6,  x7,
    x8,  x9,  x10, x11,
    x12, x13, x14, x15
} reg_t;

/*
 *      We have found that in the small RV32I core designs, the upper 16 registers consume around one
 *      quarter of the total area of the core excluding memories, thus their removal saves around 25%
 *      core area with a corresponding core power reduction.
 *
 *      This change requires a different calling convention and ABI. In particular, RV32E is only used
 *      with a soft-float calling convention. A new embedded ABI is under consideration that would
 *      work across RV32E and RV32I.
 */

int
parse_instruction(const char *cinstr)
{
    /* Let's make a copy of `cinstr` which we can then modify */
    char minstr[256];
    strcpy(minstr, cinstr);

    char *instr = strtrim(minstr);

    if ( isatty(STDOUT_FILENO) )
        printf("\n \x1b[32m>\x1b[0m \x1b[31m%s\x1b[0m\n", instr);
    else
        printf("\n > %s\n", instr);

    fprintf(stderr, "Sorry!  Instruction parsing is still unimplemented for RV32E!\n");

    return -1;
}

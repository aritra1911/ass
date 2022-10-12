#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "strtrim.h"
#include "ass.h"

#define OPCODE_LEN  3
#define COND_LEN    2
#define UR_MOM      -1  /* She so heavy, even the signed integer
                         * calculating her weight overflowed! */

typedef enum {
    EQ, NE, CS, CC,
    MI, PL, VS, VC,
    HI, LS, GE, LT,
    GT, LE, AL, NV
} cond_t;

typedef enum {
    AND,    EOR,    SUB,    RSB,
    ADD,    ADC,    SBC,    RSC,
    TST,    TEQ,    CMP,    CMN,
    ORR,    MOV,    BIC,    MVN
} opcode_t;

typedef enum {
    R0,  R1,  R2,  R3,
    R4,  R5,  R6,  R7,
    R8,  R9,  R10, FP,
    IP,  SP,  LR,  PC
} reg_t;

typedef enum {
    IMM,    /* #<immediate>             (page A5-6 ) */ /* TODO */
    REG,    /* <Rm>                     (page A5-8 ) */ /* TODO */
    LSLI,   /* <Rm>, LSL #<shift_imm>   (page A5-9 ) */ /* TODO */
    LSLR,   /* <Rm>, LSL <Rs>           (page A5-10) */ /* TODO */
    LSRI,   /* <Rm>, LSR #<shift_imm>   (page A5-11) */ /* TODO */
    LSRR,   /* <Rm>, LSR <Rs>           (page A5-12) */ /* TODO */
    ASRI,   /* <Rm>, ASR #<shift_imm>   (page A5-13) */ /* TODO */
    ASRR,   /* <Rm>, ASR <Rs>           (page A5-14) */ /* TODO */
    RORI,   /* <Rm>, ROR #<shift_imm>   (page A5-15) */ /* TODO */
    RORR,   /* <Rm>, ROR <Rs>           (page A5-16) */ /* TODO */
    RRX,    /* <Rm>, RRX                (page A5-17) */ /* TODO */
} addr_mode_t;

typedef struct {
    uint8_t rotate_imm;
    uint8_t immed_8;
} imm_t;

typedef struct {
    addr_mode_t mode;
    union {
        imm_t imm;
        reg_t rm;
    } op;
} shifter_op_t;

typedef struct {
    cond_t cond;
    opcode_t opcode;
    uint8_t s;
    uint8_t rd;
    uint8_t rn;
    shifter_op_t shifter_op;
} instruct_t;

static const char *
get_addr_mode_str(addr_mode_t mode)
{
    switch ( mode ) {
    case IMM:   return "IMM";
    case REG:   return "REG";
    case LSLI:  return "LSLI";
    case LSLR:  return "LSLR";
    case LSRI:  return "LSRI";
    case LSRR:  return "LSRR";
    case ASRI:  return "ASRI";
    case ASRR:  return "ASRR";
    case RORI:  return "RORI";
    case RORR:  return "RORR";
    case RRX:   return "RRX";
    }
}

static int
parse_reg(const char *rstr_)
{
    char rstr__[256];
    strcpy(rstr__, rstr_);
    char *rstr = strtrim(rstr__);

    reg_t ret;

         if ( rstr[0] == 'R' )      ret = strtol(rstr + 1, NULL, 10);
    else if ( !strcmp(rstr, "FP") ) ret = FP;
    else if ( !strcmp(rstr, "IP") ) ret = IP;
    else if ( !strcmp(rstr, "SP") ) ret = SP;
    else if ( !strcmp(rstr, "LR") ) ret = LR;
    else if ( !strcmp(rstr, "PC") ) ret = PC;
    else                            ret = UR_MOM;

    return ret;
}

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
        printf("\n \x1b[32m>\x1b[0m \x1b[31m%s\x1b[0m\n", instr);
    else
        printf("\n > %s\n", instr);

    char *opcode = strtok(instr, " ");
    char *rd = strtok(NULL, ", ");
    char *rn = strtok(NULL, ", ");
    char *shifter_operand = strtrim(strtok(NULL, ";")); /* TODO: Fix this */

    instruct_t instruct; memset(&instruct, 0, sizeof(instruct_t));
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
        return UR_MOM;
    }

    printf("           OpCode : '%s' (0x%x)\n", opcode, instruct.opcode);

    /*
     * Go beyond the instruction opcode to check if we got any
     * condition or status affect flag.
     */
    op += OPCODE_LEN;

    /* Parse Condition ( if there's any that is ) */
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

     if ( *op == 'S' ) instruct.s = 1;
     else if ( *op ) {
        if ( op[strlen(op) - 1] == 'S' ) op[strlen(op) - 1] = '\0';
        fprintf(stderr, "Unknown cond : '%s'\n", op - COND_LEN);
        return UR_MOM;
    }

    printf("             cond : 0x%x\n", instruct.cond);
    printf("                s : %u\n", instruct.s);


    /* Parse register values */
    int rd_val = parse_reg(rd);
    int rn_val = parse_reg(rn); /* TODO: Might not exist in case of MOV and CMP */

    printf("               Rd : '%s' (0x%x)\n", rd, rd_val);
    printf("               Rn : '%s' (0x%x)\n", rn, rn_val);
    printf("  shifter operand : '%s'\n", shifter_operand);

    /* Parse shifter operand */
    if ( *shifter_operand == 'R' && strlen(shifter_operand) == 2 ) {
        instruct.shifter_op.mode = REG;
        instruct.shifter_op.op.rm = parse_reg(shifter_operand);
    } else {
        fprintf(stderr, "Invalid (or unimplemented) shifter operand '%s'\n",
                        shifter_operand);
        return UR_MOM;
    }

    printf("  `- mode : %s\n", get_addr_mode_str(instruct.shifter_op.mode));

    return 0;
}

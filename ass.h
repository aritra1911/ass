#ifndef __ASS_H_
# define __ASS_H_

# include <stdint.h>

typedef enum {
    OPCODE, ARG, LABEL
} token_t;

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

typedef struct {
    cond_t cond;
    opcode_t opcode;
    uint32_t s;
    uint32_t rd;
    uint32_t rn;
    //shifter_t operand;
} instruct_t;

int parse_instruction(const char *inst);

#endif  /* __ASS_H_ */

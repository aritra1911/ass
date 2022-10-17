#ifndef __ASS_H_
# define __ASS_H_

# include <stdint.h>

# define UR_MOM     -1  /* She so heavy, even the signed integer
                         * calculating her weight overflowed! */

int parse_instruction(const char *inst);

int hex32el(char **hex, uint32_t, size_t spaces);

#endif  /* __ASS_H_ */

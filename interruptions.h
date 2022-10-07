#ifndef INTERRUPTIONS_H
#define INTERRUPTIONS_H

#include "stdbool.h"
#include "inttypes.h"

void        init_traitant_IT(int32_t num_IT, void (*traitant)(void));

void        traitant_IT_32();

void        masque_IRQ(uint32_t num_IRQ, bool masque);

#endif
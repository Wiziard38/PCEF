#include "traitants.h"

void        init_traitant_IT(int32_t num_IT, void (*traitant)(void))
{
    void ** ptr = (void **)0x1000;
    ptr += num_IT;
    *ptr = traitant;
}

// void        masque_IRQ(uint32_t num_IRQ, bool masque)
// {

// }


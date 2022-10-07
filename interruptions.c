#include "interruptions.h"
#include "segment.h"
#include "cpu.h"


void        init_traitant_IT(int32_t num_IT, void (*traitant)(void))
{
    uint32_t * ptr_tab = (uint32_t *)0x1000;
    ptr_tab += 2*num_IT;
    uint32_t buffer = KERNEL_CS << 16;
    buffer += ((uint32_t)traitant & 0xFFFF);
    *ptr_tab = buffer;
    ptr_tab ++;
    buffer = ((uint32_t)traitant & 0xFFFF0000);
    buffer += 0x8E00;
    *ptr_tab = buffer;
}

void        masque_IRQ(uint32_t num_IRQ, bool masque)
{
    uint8_t masques_IRQ = inb(0x21);
    uint8_t value_masque = (masques_IRQ >> num_IRQ) & 1;
    if (value_masque == 1 && masque == 0) {
        masques_IRQ -= (1 << num_IRQ);
    }
    if (value_masque == 0 && masque == 1) {
        masques_IRQ += (1 << num_IRQ);
    }
    outb(masques_IRQ, 0x21);
}


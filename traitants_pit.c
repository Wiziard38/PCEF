#include "traitants_pit.h"
#include "cpu.h"

void        init_clock(void)
{
    outb(0x34, 0x43);
    outb((QUARTZ / CLOCKFREQ) & 0xFF, 0x40);
    outb(((QUARTZ / CLOCKFREQ) >> 8) & 0xFF, 0x40);
}

void        tic_PIT(void)
{
    outb(0x20, 0x20);
    
}

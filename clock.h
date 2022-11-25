#ifndef CLOCK_H
#define CLOCK_H

#include "inttypes.h"

#define QUARTZ 0x1234DD
#define CLOCKFREQ 57

void init_clock(void);

void tic_PIT(void);

uint32_t get_nbr_ticks(void);

uint32_t get_nbr_secondes(void);

#endif
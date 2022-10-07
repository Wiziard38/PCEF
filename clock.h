#ifndef CLOCK_H
#define CLOCK_H

#define QUARTZ 0x1234DD
#define CLOCKFREQ 57

void        init_clock(void);

void        tic_PIT(void);

#endif
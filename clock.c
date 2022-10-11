#include "clock.h"
#include "cpu.h"
#include "ecran.h"
#include "inttypes.h"
#include "stdio.h"

static uint32_t CURRENT_TIME;
static uint8_t HOURS, MINUTES, SECONDS;

void init_clock(void)
{
    outb(0x34, 0x43);
    outb((QUARTZ / CLOCKFREQ) & 0xFF, 0x40);
    outb(((QUARTZ / CLOCKFREQ) >> 8) & 0xFF, 0x40);
    CURRENT_TIME = 0;
    HOURS = 0;
    MINUTES = 0;
    SECONDS = 0;
}

void tic_PIT(void)
{
    outb(0x20, 0x20);
    CURRENT_TIME += 1;
    if (CURRENT_TIME == 57) {
        CURRENT_TIME = 0;
        SECONDS += 1;
        if (SECONDS == 60) {
            SECONDS = 0;
            MINUTES += 1;
            if (MINUTES == 60) {
                MINUTES = 0;
                HOURS += 1;
            }
        }
    }
    
    char output[9];
    sprintf(output, "%02d:%02d:%02d", HOURS, MINUTES, SECONDS);
    affichage_timer(output);
}

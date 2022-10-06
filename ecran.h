#ifndef ECRAN_H
#define ECRAN_H

#include "inttypes.h"

#define WIDTH 80
#define HEIGHT 25
#define CONST_CAR 0x0F20
#define INIT_PTR 0xB8000

enum COLORS {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    YELLOW,
    WHITE
};

uint16_t     *ptr_mem(uint32_t lig,  uint32_t col);

void        ecrit_car(uint32_t lig,     uint32_t col,   char c);

void        efface_ecran(void);

void        place_curseur(uint32_t lig,     uint32_t col);

void        deplace_curseur(uint32_t delta_lig,     uint32_t delta_col);

void        affiche_curseur(void);

void        traite_car(char c);

void        defilement(void);

void        console_putbytes(const char *s,     int len);

void        affichage_timer(const char *s);

#endif
#ifndef ECRAN_H
#define ECRAN_H

#include "inttypes.h"

#define WIDTH 80
#define HEIGHT 25
#define CONST_CAR 30752
#define INIT_PTR 0xB8000
#define DEFAULT_BACK_COLOR 0
#define DEFAULT_TEXT_COLOR 15


int16_t     *ptr_mem(uint32_t lig,  uint32_t col);

void        ecrit_car(uint32_t lig,     uint32_t col,   char c,    int8_t background_color,   int8_t color);

void        efface_ecran(void);

void        place_curseur(uint32_t lig,     uint32_t col);

void        affiche_curseur(void);

void        traite_car(char c);

void        defilement(void);

void        console_putbytes(const char *s,     int len);


#endif
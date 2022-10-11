#include "ecran.h"
#include "cpu.h"
#include "string.h"
#include <stdio.h>

static int32_t LIGNE_CURRENT = 0;
static int32_t COLONNE_CURRENT = 0;
static int8_t TEXT_BACK_COLOR = BLACK;
static int8_t TEXT_COLOR = WHITE;


uint16_t *ptr_mem(uint32_t lig, uint32_t col) 
{
    return (uint16_t *)(INIT_PTR + 2*(lig*80 + col));    
}

void ecrit_car(uint32_t lig, uint32_t col, char c)
{
    uint16_t* ptr = ptr_mem(lig, col);
    int16_t res = 0;
    res += (TEXT_BACK_COLOR << 12);
    res += (TEXT_COLOR << 8);
    res += c;

    *ptr = res;
}

void efface_ecran(void)
{
    int16_t* ptr = (int16_t*)INIT_PTR;
    for (int16_t i = 0; i < (HEIGHT * WIDTH); i++) {
        *ptr++ = CONST_CAR;
    }
    place_curseur(0, 0);
}

void place_curseur(uint32_t lig, uint32_t col)
{
    LIGNE_CURRENT = lig,
    COLONNE_CURRENT = col;
    affiche_curseur();
}

void deplace_curseur(uint32_t delta_lig, uint32_t delta_col)
{
    LIGNE_CURRENT = ((COLONNE_CURRENT + delta_col) / WIDTH + LIGNE_CURRENT + delta_lig);
    COLONNE_CURRENT = (COLONNE_CURRENT + delta_col) % WIDTH;
    if (LIGNE_CURRENT >= HEIGHT) {
        LIGNE_CURRENT = HEIGHT - 1;
        COLONNE_CURRENT = WIDTH - 1;
    }
    affiche_curseur();
}

void affiche_curseur(void)
{
    uint16_t pos = LIGNE_CURRENT * WIDTH + COLONNE_CURRENT;
    uint8_t tmp;

    // Partie basse
    outb(0x0F, 0x3D4);
    tmp = pos & 0x00FF;
    outb(tmp, 0x3D5);

    // Partie haute
    outb(0x0E, 0x3D4);
    tmp = (uint8_t)(pos & 0xFF00);
    outb(tmp, 0x3D5);
}


void traite_car(char c)
{   
    if (32 <= c && c <= 126) {
        ecrit_car(LIGNE_CURRENT, COLONNE_CURRENT, c);
        deplace_curseur(0, 1);
    } 
    
    switch (c) {
    case 8:
        if (COLONNE_CURRENT != 0) {
            place_curseur(LIGNE_CURRENT, COLONNE_CURRENT - 1);
        }
        break;
    
    case 9:
        int32_t new_col = COLONNE_CURRENT + 8 - COLONNE_CURRENT % 8;
        new_col = (COLONNE_CURRENT == 80) ? 79 : COLONNE_CURRENT;
        place_curseur(LIGNE_CURRENT, new_col);
        break;
    
    case 10:
        int32_t new_line = (LIGNE_CURRENT == 24) ? 24 : LIGNE_CURRENT + 1;
        place_curseur(new_line, 0);
        break;
    
    case 12:
        efface_ecran();
        place_curseur(0, 0);
        break;
    
    case 13:
        place_curseur(LIGNE_CURRENT, 0);
        break;
    
    default:
        break;
    }
}

void defilement(void)
{
    uint16_t * mem_from = ptr_mem(0, 0);
    uint16_t * mem_to = ptr_mem(1, 0);
    memmove(mem_from, mem_to, WIDTH*HEIGHT*2);
    LIGNE_CURRENT -= 1;
    affiche_curseur();
}

void console_putbytes(const char *s, int len)
{
    for (int i = 0; i < len; i ++) {
        traite_car(s[i]);
    }
}

void affichage_timer(const char *s)
{
    TEXT_BACK_COLOR = DARK_GRAY;
    int32_t lig_mem = LIGNE_CURRENT;
    int32_t col_mem = COLONNE_CURRENT;
    place_curseur(0, WIDTH - 10);
    traite_car(' ');
    console_putbytes(s, 8);
    traite_car(' ');
    place_curseur(lig_mem, col_mem);
    TEXT_BACK_COLOR = BLACK;
}

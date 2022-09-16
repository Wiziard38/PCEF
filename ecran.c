#include "ecran.h"
#include "cpu.h"

static int32_t LIGNE_CURRENT = 0;
static int32_t COLONNE_CURRENT = 0;


int16_t     *ptr_mem(uint32_t lig,  uint32_t col) 
{
    return (int16_t*)(INIT_PTR + 2*(lig*80 + col));    
}


void        ecrit_car(uint32_t lig,    uint32_t col,    char c,    int8_t background_color,   int8_t color)
{
    int16_t* ptr = ptr_mem(lig, col);
    int16_t res = 0;
    res += (background_color << 12);
    res += (color << 8);
    res += c;

    *ptr = res;
}


void        efface_ecran(void)
{
    int16_t* ptr = (int16_t*)INIT_PTR;
    for (int16_t i = 0; i < (HEIGHT * WIDTH); i++) {
        *ptr++ = 0;
    }
}


void        place_curseur(uint32_t lig,     uint32_t col)
{
    LIGNE_CURRENT = lig,
    COLONNE_CURRENT = col;
    affiche_curseur();
}


void        affiche_curseur(void)
{
    uint16_t pos = LIGNE_CURRENT * 80 + COLONNE_CURRENT;
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


void        traite_car(char c)
{   
    if (32 <= c && c <= 126) {
        ecrit_car(LIGNE_CURRENT, COLONNE_CURRENT, c, DEFAULT_BACK_COLOR, DEFAULT_TEXT_COLOR);
    } 
    
    switch (c) {
    case 8:
        if (COLONNE_CURRENT != 0) {
            COLONNE_CURRENT -= 1;
        }
        break;
    
    case 9:
        COLONNE_CURRENT = ((COLONNE_CURRENT / 8) + 1) * 8;
        if (COLONNE_CURRENT == 80) {
            COLONNE_CURRENT = 79;
        }
        break;
    
    case 10:
        COLONNE_CURRENT = 0;
        LIGNE_CURRENT += 1;
        break;
    
    case 12:
        efface_ecran();
        COLONNE_CURRENT = 0;
        LIGNE_CURRENT = 0;
        break;
    
    case 13:
        COLONNE_CURRENT = 0;
        break;
    
    default:
        break;
    }

    affiche_curseur();
}
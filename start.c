#include <cpu.h>
#include <inttypes.h>
#include "ecran.h"

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display


void kernel_start(void)
{
    ecrit_car(10, 30, 99, 2, 11);
    efface_ecran();
    ecrit_car(10, 40, 104, 2, 11);

    place_curseur(1, 1);


    // quand on saura gerer l'ecran, on pourra afficher x
    //(void)x;
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}


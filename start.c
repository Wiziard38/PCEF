#include <cpu.h>
#include <inttypes.h>
#include "ecran.h"
#include "stdio.h"

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display


void kernel_start(void)
{
    efface_ecran();
    printf("test");
    
    // quand on saura gerer l'ecran, on pourra afficher x
    //(void)x;
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}


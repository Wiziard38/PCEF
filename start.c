#include <cpu.h>
#include <inttypes.h>
#include "ecran.h"
#include "stdio.h"
#include "clock.h"
#include "interruptions.h"
#include "processus.h"

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display


void kernel_start(void)
{
    efface_ecran();
    
    /* Initialisations des traitants */
    init_traitant_IT(32, traitant_IT_32);
    
    init_clock();
    
    /* Initialisations des processus */
    init_process();

    idle();

    // masque_IRQ(0, 0);
    // démasquage des interruptions externes
    // sti();
    // boucle d’attente
    // while (1) {
    //     hlt();
    // }
}


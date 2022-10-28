#include "processus.h"
#include "string.h"
#include "stdio.h"
#include "cpu.h"
#include <stdlib.h>
#include "tinyalloc.h"

static uint32_t CURRENT_INDEX_PROC = 0;

Processus * PROCESS_TABLE[NUMBER_PROCESS];
Processus_linked list_processus;


void init_processes(void)
{
    init_idle_processus();
    for (int i = 1; i <= NUMBER_PROCESS - 1; i++) {
        char process_name[20] = "";
        sprintf(process_name, "proc%d", i);
        cree_processus(&proc, process_name);
        PROCESS_TABLE[i-1]->suivant = PROCESS_TABLE[i];
    }

    list_processus.first = PROCESS_TABLE[0];
    list_processus.last = PROCESS_TABLE[NUMBER_PROCESS - 1];
}


int32_t cree_processus(void (*code)(void), char *nom) {
    if (CURRENT_INDEX_PROC >= NUMBER_PROCESS) {
        printf("Trop de processus !");
        return -1;
    }
    Processus * new_process = malloc(sizeof(Processus));
    new_process->pid = CURRENT_INDEX_PROC;
    strcpy(new_process->processus_name, nom);
    new_process->processus_state = WAITING;
    new_process->save_zone[1] = (uint32_t) (&(new_process->pile_exec[511]));
    new_process->pile_exec[511] = (uint32_t) code;

    PROCESS_TABLE[CURRENT_INDEX_PROC] = new_process;
    CURRENT_INDEX_PROC++;
    return new_process->pid;
}


int32_t init_idle_processus(void)
{
    if (CURRENT_INDEX_PROC != 0) {
        printf("Idle non premier !");
        return -1;
    }
    Processus * idle_process = malloc(sizeof(Processus));
    idle_process->pid = 0;
    strcpy(idle_process->processus_name, "idle");
    idle_process->processus_state = RUNNING;
    
    PROCESS_TABLE[0] = idle_process;
    CURRENT_INDEX_PROC++;
    return 0;
}

//         ctx_sw(&(PROCESS_TABLE[0].save_zone[0]), &(PROCESS_TABLE[1].save_zone[0]));

void idle(void)
{
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
}

void proc(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
}

void ordonnance(void)
{
    Processus * current = list_processus.first;
    list_processus.first = current->suivant;
    list_processus.last->suivant = current;
    list_processus.last = current;
    current->processus_state = WAITING;
    list_processus.first->processus_state = RUNNING;
    ctx_sw(current->save_zone, list_processus.first->save_zone);
}

int32_t mon_pid(void)
{
    return list_processus.first->pid;
}

char *mon_nom(void)
{
    return list_processus.first->processus_name;
}

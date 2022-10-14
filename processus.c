#include "processus.h"
#include "string.h"
#include "stdio.h"
#include "cpu.h"
#include <stdlib.h>
#include "tinyalloc.h"

static uint32_t CURRENT_INDEX_PROC = 0;

Processus PROCESS_TABLE[NUMBER_PROCESS];
Processus_linked list_processus;


void init_process(void)
{
    create_processus(0, "idle", elu);
    create_processus(1, "proc1", activable);
    PROCESS_TABLE[0].suivant = &(PROCESS_TABLE[1]);
    PROCESS_TABLE[1].save_zone[1] = (uint32_t) &PROCESS_TABLE[1].pile_exec[511];
    PROCESS_TABLE[1].pile_exec[511] = (uint32_t) &proc1;
    list_processus.first = &PROCESS_TABLE[0];
    list_processus.last = &PROCESS_TABLE[1];
}


void create_processus(int32_t pid, char * processus_name, Processus_states processus_state)
{
    if (CURRENT_INDEX_PROC >= NUMBER_PROCESS) {
        printf("Trop de processus !");
    } else {
    Processus current_process;
    current_process.pid = pid;
    strcpy(current_process.processus_name, processus_name);
    current_process.processus_state = processus_state;
    PROCESS_TABLE[CURRENT_INDEX_PROC] = current_process;
    CURRENT_INDEX_PROC++;
    }
}

//         ctx_sw(&(PROCESS_TABLE[0].save_zone[0]), &(PROCESS_TABLE[1].save_zone[0]));

void idle(void)
{
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        ordonnance();
    }
}

void proc1(void) {
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
    current->processus_state = activable;
    list_processus.first->processus_state = elu;
    ctx_sw(&(current->save_zone[0]), &(list_processus.first->save_zone[0]));
}

Processus * PROCESS_TABLE_get(void)
{
    return PROCESS_TABLE;
}

int32_t mon_pid(void)
{
    return list_processus.first->pid;
}

char *mon_nom(void)
{
    return list_processus.first->processus_name;
}

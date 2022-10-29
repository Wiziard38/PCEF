#include "processus.h"
#include "string.h"
#include "stdio.h"
#include "cpu.h"
#include <stdlib.h>
#include "tinyalloc.h"
#include "clock.h"

static uint32_t CURRENT_INDEX_PROC = 0;

Processus * PROCESS_TABLE[NUMBER_PROCESS];
Processus_linked linked_waiting_process;
Processus_linked linked_sleeping_process;


void init_processes(void) {
    init_idle_processus();
    for (int i = 1; i <= NUMBER_PROCESS - 1; i++) {
        char process_name[20] = "";
        sprintf(process_name, "proc%d", i);
        cree_processus(&proc, process_name);
        PROCESS_TABLE[i-1]->suivant = PROCESS_TABLE[i];
    }

    linked_waiting_process.first = PROCESS_TABLE[0];
    linked_waiting_process.last = PROCESS_TABLE[NUMBER_PROCESS - 1];
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
    new_process->sleeping_time = 0;
    new_process->save_zone[1] = (uint32_t) (&(new_process->pile_exec[511]));
    new_process->pile_exec[511] = (uint32_t) code;

    PROCESS_TABLE[CURRENT_INDEX_PROC] = new_process;
    CURRENT_INDEX_PROC++;
    return new_process->pid;
}


int32_t init_idle_processus(void) {
    if (CURRENT_INDEX_PROC != 0) {
        printf("Idle non premier !");
        return -1;
    }
    Processus * idle_process = malloc(sizeof(Processus));
    idle_process->pid = 0;
    strcpy(idle_process->processus_name, "idle");
    idle_process->processus_state = RUNNING;
    idle_process->sleeping_time = 0;
    
    PROCESS_TABLE[0] = idle_process;
    CURRENT_INDEX_PROC++;
    return 0;
}

void idle(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void proc(void) {
    for (;;) {
        printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        sti();
        hlt();
        cli();
    }
}

void ordonnance(void) {
    insert_tail_process(extract_head_process(linked_waiting_process), linked_waiting_process);
    while (linked_sleeping_process.first->sleeping_time < get_nbr_ticks()) {
        Processus * waking_process = extract_head_process(linked_sleeping_process);
        waking_process->sleeping_time = 0;
        waking_process->processus_state = WAITING;
        insert_tail_process(waking_process, linked_waiting_process);
    }
    ctx_sw(linked_waiting_process.last->save_zone, linked_waiting_process.first->save_zone);
}

int32_t mon_pid(void) {
    return linked_waiting_process.first->pid;
}

char *mon_nom(void) {
    return linked_waiting_process.first->processus_name;
}

struct Processus *extract_head_process(struct Processus_linked type_linked_process) {
    Processus * current_process = type_linked_process.first;
    type_linked_process.first = current_process->suivant;
    current_process->processus_state = WAITING;
    type_linked_process.first->processus_state = RUNNING;
    return current_process;
}

void insert_tail_process(struct Processus *current_process, struct Processus_linked type_linked_process) {
    type_linked_process.last->suivant = current_process;
    type_linked_process.last = current_process;
}


void dors(uint32_t nbr_secs) {
    Processus * process = extract_head_process(linked_waiting_process);
    process->sleeping_time = nbr_secs + get_nbr_ticks();
    process->processus_state = SLEEPING;
    if (linked_sleeping_process.first == NULL) {
        linked_sleeping_process.first = process;
        return;
    } else {
        Processus * current_process = linked_waiting_process.first;
        if (current_process->sleeping_time > process->sleeping_time) {
            process->suivant = current_process;
            linked_sleeping_process.first = process;
        } else {
            while (current_process->suivant->sleeping_time > process->sleeping_time) {
                if (current_process->suivant == linked_sleeping_process.last) {
                    linked_sleeping_process.last->suivant = process;
                    linked_sleeping_process.last = process;
                    return;
                }
                current_process = current_process->suivant;
            }
            process->suivant = current_process->suivant;
            current_process->suivant = process;
        }
    }
}

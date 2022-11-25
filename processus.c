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
Processus_linked linked_killing_process;


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

void idle() {
    for (;;) {
        //printf("idle \n");
        sti();
        hlt();
        cli();
    }
}

void proc1(void) {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", get_nbr_secondes(),
        mon_nom(), mon_pid());
        dors(2);
    }
}

void proc2(void) {
    for (int i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", get_nbr_secondes(),
        mon_nom(), mon_pid());
        dors(3);
    }
    fin_processus();
}

void proc3(void) {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n", get_nbr_secondes(),
        mon_nom(), mon_pid());
        dors(5);
    }
}

void init_processes(void) {
    init_idle_processus();
    cree_processus(&proc1, "proc1");
    cree_processus(&proc2, "proc2");
    cree_processus(&proc3, "proc3");

    for (int i = 1; i <= NUMBER_PROCESS - 1; i++) {
        PROCESS_TABLE[i-1]->suivant = PROCESS_TABLE[i];
    }

    linked_waiting_process.first = PROCESS_TABLE[0];
    linked_waiting_process.last = PROCESS_TABLE[NUMBER_PROCESS - 1];
}


void ordonnance(void) {
    while (linked_sleeping_process.first != NULL && linked_sleeping_process.first->sleeping_time < get_nbr_ticks()) {
        Processus * waking_process = extract_head_process(&linked_sleeping_process);
        waking_process->sleeping_time = 0;
        waking_process->processus_state = WAITING;
        insert_tail_process(waking_process, &linked_waiting_process);
    }

    if (linked_waiting_process.first->suivant != NULL) {
        insert_tail_process(extract_head_process(&linked_waiting_process), &linked_waiting_process);
        ctx_sw(linked_waiting_process.last->save_zone, linked_waiting_process.first->save_zone);
    }

    while (linked_killing_process.first != NULL) {
        Processus * to_kill_process = extract_head_process(&linked_killing_process);
        free(to_kill_process);
    }
}

int32_t mon_pid(void) {
    return linked_waiting_process.first->pid;
}

char *mon_nom(void) {
    return linked_waiting_process.first->processus_name;
}

struct Processus *extract_head_process(struct Processus_linked *type_linked_process) {
    Processus * current_process = (*type_linked_process).first;
    if (current_process->suivant == NULL) {
        (*type_linked_process).first = NULL;
    } else {
        (*type_linked_process).first = current_process->suivant;
        (*type_linked_process).first->processus_state = RUNNING;
    }
    current_process->processus_state = WAITING;
    current_process->suivant = NULL;
    return current_process;
}

void insert_tail_process(struct Processus *current_process, struct Processus_linked *type_linked_process) {
    (*type_linked_process).last->suivant = current_process;
    (*type_linked_process).last = current_process;
}


void dors(uint32_t nbr_secs) {
    Processus * to_place_process = extract_head_process(&linked_waiting_process);
    to_place_process->sleeping_time = nbr_secs*60 + get_nbr_ticks();
    to_place_process->processus_state = SLEEPING;

    if (linked_sleeping_process.first == NULL) {
        linked_sleeping_process.first = to_place_process;
        linked_sleeping_process.last = to_place_process;
        to_place_process->suivant = NULL;
        ctx_sw(to_place_process->save_zone, linked_waiting_process.first->save_zone);
        return;
    }

    Processus * current_process = linked_sleeping_process.first;
    
    if (current_process->sleeping_time > to_place_process->sleeping_time) {
        to_place_process->suivant = current_process;
        linked_sleeping_process.first = to_place_process;
        ctx_sw(to_place_process->save_zone, linked_waiting_process.first->save_zone);
        return;    
    }
    
    while (current_process->suivant != NULL) {
        if (current_process->suivant->sleeping_time > to_place_process->sleeping_time) {
            to_place_process->suivant = current_process->suivant;
            current_process->suivant = to_place_process;
            ctx_sw(to_place_process->save_zone, linked_waiting_process.first->save_zone);
            return;
        }
        current_process = current_process->suivant;
    }

    to_place_process->suivant = NULL;
    linked_sleeping_process.last->suivant = to_place_process;
    linked_sleeping_process.last = to_place_process;
    ctx_sw(to_place_process->save_zone, linked_waiting_process.first->save_zone);
    return;
}

void fin_processus(void) {
    printf("Killing processus %s pid = %i\n", mon_nom(), mon_pid());
    Processus * to_kill_process = extract_head_process(&linked_waiting_process);
    to_kill_process->processus_state = DYING;
    insert_tail_process(to_kill_process, &linked_killing_process);
    ctx_sw(to_kill_process->save_zone, linked_waiting_process.first->save_zone);
}

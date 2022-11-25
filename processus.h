#ifndef PROCESSUS_H
#define PROCESSUS_H

#include "inttypes.h"

#define PILE_SIZE_PROCESS 512
#define NUMBER_PROCESS 4

typedef enum Processus_states {
    RUNNING,
    WAITING,
    SLEEPING,
} Processus_states;

typedef struct Processus {
    int32_t pid;
    char processus_name[20];
    Processus_states processus_state;
    uint32_t save_zone[5];
    uint32_t pile_exec[PILE_SIZE_PROCESS];
    uint32_t sleeping_time;
    struct Processus * suivant;
} Processus;

typedef struct Processus_linked {
    Processus * first;
    Processus * last;
} Processus_linked;

void ctx_sw(uint32_t * old_process, uint32_t * new_process);

void init_processes(void);

int32_t cree_processus(void (*code)(void), char *nom);

int32_t init_idle_processus(void);

void idle(void);

void proc(void);

void ordonnance(void);

int32_t mon_pid(void);

char *mon_nom(void);

struct Processus *extract_head_process(struct Processus_linked *type_linked_process);

void insert_tail_process(struct Processus *current_process, struct Processus_linked *type_linked_process);

void dors(uint32_t nbr_secs);

#endif
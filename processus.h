#ifndef PROCESSUS_H
#define PROCESSUS_H

#include "inttypes.h"

#define PILE_SIZE_PROCESS 512
#define NUMBER_PROCESS 2

typedef enum Processus_states {
    elu,
    activable,
} Processus_states;

typedef struct Processus {
    int32_t pid;
    char processus_name[20];
    Processus_states processus_state;
    uint32_t save_zone[5];
    uint32_t pile_exec[PILE_SIZE_PROCESS];
} Processus;

Processus PROCESS_TABLE[NUMBER_PROCESS];

void ctx_sw(uint32_t * old_process, uint32_t * new_process);

void init_processus(int32_t pid, char * processus_name, Processus_states processus_state);

void idle(void);

void proc1(void);

#endif
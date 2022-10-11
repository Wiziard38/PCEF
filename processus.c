#include "processus.h"
#include "string.h"
#include "stdio.h"
#include "cpu.h"
#include <stdlib.h>

Processus PROCESS_TABLE[NUMBER_PROCESS];
static uint32_t CURRENT_INDEX_PROC = 0;


void init_processus(int32_t pid, char processus_name[20], Processus_states processus_state)
{
    if (CURRENT_INDEX_PROC >= NUMBER_PROCESS) {
        printf("trop de processus !");
    } else {
    Processus current_process;
    current_process.pid = pid;
    strcpy(current_process.processus_name, processus_name);
    current_process.processus_state = processus_state;
    PROCESS_TABLE[CURRENT_INDEX_PROC] = current_process;
    CURRENT_INDEX_PROC++;
    }
}


void idle(void)
{
    printf("[idle] je tente de passer la main a proc1...\n");
    //ctx_sw(..., ...);
}


void proc1(void)
{
    printf("[proc1] idle m'a donne la main\n");
    printf("[proc1] j'arrete le systeme\n");
    hlt();
}
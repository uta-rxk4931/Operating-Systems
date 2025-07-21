#pragma once

#include "elf.h"
#include "disk.h"

enum {
    PROC_UNUSED,
    PROC_LOADING, /* allocated and wait for loading elf binary */
    PROC_READY,   /* finished loading elf and wait for first running */
    PROC_RUNNING,
    PROC_RUNNABLE,
    PROC_WAIT_TO_SEND,
    PROC_WAIT_TO_RECV
};

struct process{
    int pid;
    int status;
    int receiver_pid; /* used when waiting to send a message */
    void *sp, *mepc;  /* process context = stack pointer (sp)
                       * + machine exception program counter (mepc) */
    int tickets[16];/* Student's code goes here to add to the process control block (priority / scheduler)*/
};

#define MAX_NPROCESS  16
extern int proc_curr_idx;
extern struct process proc_set[MAX_NPROCESS];
#define curr_pid      proc_set[proc_curr_idx].pid
#define curr_status   proc_set[proc_curr_idx].status

void timer_init();
void timer_reset();
long long mtime_get();

void proc_init();
int  proc_alloc();
void proc_free(int);
void proc_set_ready (int);
void proc_set_running (int);
void proc_set_runnable (int);
int  proc_get_pid( );
void srand(unsigned int seed);
unsigned int rand();
int proc_set_ticket(int ticket); //Added for ticket functionality -changed
int  proc_kill(int pid); //Added for kill functionality
struct process * proc_get_proc_set( );

void ctx_entry(void);
void ctx_start(void** old_sp, void* new_sp);
void ctx_switch(void** old_sp, void* new_sp);

/*
 * (C) 2022, Cornell University
 * All rights reserved.
 */

/* Author: Yunhao Zhang
 * Description: helper functions for managing processes
 */

#include "egos.h"
#include "process.h"
#include "syscall.h"
#include <string.h>

static int s_iSeed = 0;

void intr_entry(int id);

void excp_entry(int id) {
    /* Student's code goes here (handle memory exception). */

    /* If id is for system call, handle the system call and return */

    /* Otherwise, kill the process if curr_pid is a user application */

    /* Student's code ends here. */

    FATAL("excp_entry: kernel got exception %d", id);
}

void proc_init() {
    earth->intr_register(intr_entry);
    earth->excp_register(excp_entry);

    /* Student's code goes here (PMP memory protection). */

    /* Setup PMP TOR region 0x00000000 - 0x20000000 as r/w/x */

    /* Setup PMP NAPOT region 0x20400000 - 0x20800000 as r/-/x */

    /* Setup PMP NAPOT region 0x20800000 - 0x20C00000 as r/-/- */

    /* Setup PMP NAPOT region 0x80000000 - 0x80004000 as r/w/- */

    /* Student's code ends here. */
    /* Initialize the random number generator */
    srand((unsigned) grass->gettime());

    /* The first process is currently running */
    proc_set_running(proc_alloc());
}

static void proc_set_status(int pid, int status) {
    for (int i = 0; i < MAX_NPROCESS; i++)
        if (proc_set[i].pid == pid) proc_set[i].status = status;
}

int proc_alloc() {
    static int proc_nprocs = 0;
    for (int i = 0; i < MAX_NPROCESS; i++)
        if (proc_set[i].status == PROC_UNUSED) {
            proc_set[i].pid = ++proc_nprocs;
            proc_set[i].status = PROC_LOADING;
            for(int j=0;j<16;j++) //Added for ticket functionality
            {
                proc_set[i].tickets[j] = -1;      
            }
            proc_set[i].tickets[0] = rand() % 50;
        
            return proc_nprocs;
        }

    FATAL("proc_alloc: reach the limit of %d processes", MAX_NPROCESS);
}

void proc_free(int pid) {
    if (pid != -1) {
        earth->mmu_free(pid);
        proc_set_status(pid, PROC_UNUSED);
        return;
    }

    /* Free all user applications */
    for (int i = 0; i < MAX_NPROCESS; i++)
        if (proc_set[i].pid >= GPID_USER_START &&
            proc_set[i].status != PROC_UNUSED) {
            earth->mmu_free(proc_set[i].pid);
            proc_set[i].status = PROC_UNUSED;
        }
}

// implemented from http://www.jstatsoft.org/v08/i14/paper
// https://en.wikipedia.org/wiki/Xorshift
void srand(unsigned int seed)
{
    s_iSeed = seed;
}

// implemented from http://www.jstatsoft.org/v08/i14/paper
// https://en.wikipedia.org/wiki/Xorshift
unsigned int rand()
{
    s_iSeed ^= s_iSeed << 13;
    s_iSeed ^= s_iSeed >> 17;
    s_iSeed ^= s_iSeed << 5;
    return s_iSeed;
}

void proc_set_ready(int pid) { proc_set_status(pid, PROC_READY); }
void proc_set_running(int pid) { proc_set_status(pid, PROC_RUNNING); }
void proc_set_runnable(int pid) { proc_set_status(pid, PROC_RUNNABLE); }
int  proc_get_pid( ){ return curr_pid; }
struct process * proc_get_proc_set( ){ return &proc_set[0];}

int proc_set_ticket(int tickets) //Added for ticket functionality
{
    int i;
    for(i=0;i<16;i++)
    {
        proc_set[proc_curr_idx].tickets[i] = -1;      
    }
    for(i=0;i<tickets;i++)
    {
        proc_set[proc_curr_idx].tickets[i] = rand() % 50;      
    }
    
}

int proc_kill(int pid) //Added for kill functionality
{
     for(int i = 0; i < MAX_NPROCESS; i++)
    {
        if((proc_set[i].pid > 4) && (proc_set[i].pid != pid))
        {
            grass->proc_free(proc_set[i].pid);
        }
    }
}
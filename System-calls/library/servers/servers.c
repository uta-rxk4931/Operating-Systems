/*
 * (C) 2022, Cornell University
 * All rights reserved.
 */

/* Author: Yunhao Zhang
 * Description: user friendly interfaces of system processes
 * including GPID_PROCESS, GPID_FILE and GPID_DIR
 */

#include "egos.h"
#include "servers.h"
#include <string.h>

static int sender;
static char buf[SYSCALL_MSG_LEN];

void exit(int status) {
    grass->sys_exit(status);
    while(1);
}



int dir_lookup(int dir_ino, char* name) {
    struct dir_request req;
    req.type = DIR_LOOKUP;
    req.ino = dir_ino;
    strcpy(req.name, name);
    grass->sys_send(GPID_DIR, (void*)&req, sizeof(req));

    grass->sys_recv(&sender, buf, SYSCALL_MSG_LEN);
    if (sender != GPID_DIR) FATAL("dir_lookup: an error occurred");
    struct dir_reply *reply = (void*)buf;

    return reply->status == DIR_OK? reply->ino : -1;
}

int file_read(int file_ino, int offset, char* block) {
    struct file_request req;
    req.type = FILE_READ;
    req.ino = file_ino;
    req.offset = offset;
    grass->sys_send(GPID_FILE, (void*)&req, sizeof(req));

    grass->sys_recv(&sender, buf, SYSCALL_MSG_LEN);
    if (sender != GPID_FILE) FATAL("file_read: an error occurred");
    struct file_reply *reply = (void*)buf;
    memcpy(block, reply->block.bytes, BLOCK_SIZE);

    return reply->status == FILE_OK? 0 : -1;
}

int setprio( int pid, int priority )
{
  /* Student's code goes here to send a request to set a processes priority */
  /* and read and return the status */
  struct proc_request req;
  req.type = PROC_SETPRIO;

  req.argc = 2;
  req.argv[0][0] = pid;
  req.argv[1][0] = priority;
  // printf("SENDING PID %d TO PRIO %d\n", req.argv[0][0], req.argv[1][0]);
  grass->sys_send(GPID_PROCESS, (void*)&req, sizeof(req)); // SENDS TO SYS_PROC

  struct proc_reply *reply = (void*)buf;
  grass->sys_recv(NULL, (void*)&reply, sizeof(reply));
  return 1;
}

int kill(int pid) //Added for kill functionality
{
    struct proc_request req;
    struct proc_reply reply;

    req.type=PROC_KILL;

    req.argc=1;
    req.argv[0][0]=pid;

    grass->sys_send(GPID_PROCESS, (void*)&req, sizeof(req));
    grass->sys_recv(NULL, (void*)&reply, sizeof(reply));
    return 1;
}
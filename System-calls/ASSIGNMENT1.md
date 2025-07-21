# Assignment 1: System Calls and libc

For this assignment you will be making modifications to the egos-2000 operating system.  You will be implementing a new system call, a new libc function and a few assorted utility programs.

You may complete this assignment in groups of up to two people. If you wish to be in a group you must add your group to the spreadsheet linked in the Canvas assignment no later than 09/20/23 at at 11:59pm. 

## Requirements

### Default priority
All processes must start with a default priority of 2, except for the four kernel processes. The kernel processes start with a priority of 1.

### Implement setprio system call
Implement the setprio system call to allow processes to set their priority. The priority must be between 1 and 10.  

### Process control blocks
To the process control block add:
1. Priority, an integer from 1 to 10
2. The time the process was created
3. Number of context switches, an integer representing the nunmber of times a process is scheduled to be run by the scheduler

### kill system call

Add a system call, kill, that takes a PID as a command line parameter and kills the process that has the user supplied PID.  You will need to add a new type of proc_req: PROC_KILL and the associated code in servers.c and sys_proc.c

### kill application

Add a user application kill.c that takes a PID as a command line parameter and kills the process that has the user supplied PID.

You will need edit tools/mkfs.c to add your new executable to disk image.  

### ps 
Add functionality to the ps command in ps.c to display:
1. Process priority
2. Number of context switches

```shell
➜ /home/cse3320 ps
PID     STATUS  PRIORITY  CTX
1       11      1         50    
2       5       2         23
3       4       1         49
4       4       3         12
6       4       2         19
......
```

## How to submit code and report.

Your program source code is to be turned in via a push to your assignment repository. Do not clone the assignment repository.  Use the link in the Canvas assignment. Submission time is determined by the GitHub system time. You may push your code as often as you wish. Only your last submission will be graded.

## Administrative

This assignment must be coded in C. Any other language will result in 0 points. Your programs will be compiled and graded on the course GitHub Codespace. Code that does not compile with the provided makefile will result in a 0.

## Academic Integrity

This assignment must be 100% your own work. No code may be copied from friends, previous students, books, web pages, etc. All code submitted is automatically checked against a database of previous semester’s graded assignments, current student’s code and common web sources. By submitting your code on GitHub you are attesting that you have neither given nor received unauthorized assistance on this work. Code that is copied from an external source or used as inspiration, excluding the course github, will result in a 0 for the assignment and referral to the Office of Student Conduct.

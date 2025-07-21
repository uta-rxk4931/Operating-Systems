# Assignment 3: Scheduler

For this assignment you will be making modifications to the egos-2000 operating system.  You will be modifying the scheduler to use a lottery scheduling algorithm

## Requirements

### Lottery Scheduler
Kernel processes GPID_PROC, GPID_FILE and GPID_DIR will continue to be scheduled as they are now.  

The GPID_SHELL process (PID 4) and all user processes will be scheduled via a lottery scheduler.  

If none of the processes GPID_PROC, GPID_FILE and GPID_DIR are runnable then the scheduler will utilize a lottery scheduling algorithm to pick the next process, PID 4+, to run.

Each process will be created with a single lottery ticket.  The scheduler will randomly select a lottery ticket.  The process holding that lottery ticket will be scheduled to run.

You must use rand() to select your process and srand to seed the pseudo random number generator.

```
   /* Initialize the random number generator */
   srand((unsigned) grass->gettime());

   /* Print 5 random numbers from 0 to 49 */
   for( i = 0 ; i < 5 ; i++ ) {
      printf("%d\n", rand() % 50);
   }
```

### Implement setticketcount system call
Implement a system call to allow processes to set the ticket count of that process.  0 < number_tickets < 16.

A stub has been provided for you in servers.c

```
int settickets(int tickets )
{
    // set the number of tickets for the calling process
}
```

### Implement a program k
Implement a program k in apps/user/ that kills all running programs with a PID > 4

### Testing your scheduler
There is a program tick that you can run that will allow you to set the number of lottery tickets owned by that process.  It can be run as:
```
tick [ID] [tickets] &
```
So, to run a process with ID 42 and 10 tickets run:

```
tick 42 10 &
```
You can run multiple tick processes with different amounts of lottery tickets and verify their run frequency by their output. For example:
```
tick 11 10 &
tick 12 5 &
```
Your output should show process 11 running twice as often as process ID 12

## How to submit code.

Your program source code is to be turned in via a push to your assignment repository. Do not clone the assignment repository.  Use the link in the Canvas assignment. Submission time is determined by the GitHub system time. You may push your code as often as you wish. Only your last submission will be graded.

## Administrative

This assignment must be coded in C. Any other language will result in 0 points. Your programs will be compiled and graded on the course GitHub Codespace. Code that does not compile with the provided makefile will result in a 0.

## Academic Integrity

This assignment must be 100% your own work. No code may be copied from friends, previous students, books, web pages, etc. All code submitted is automatically checked against a database of previous semester’s graded assignments, current student’s code and common web sources. By submitting your code on GitHub you are attesting that you have neither given nor received unauthorized assistance on this work. Code that is copied from an external source or used as inspiration, excluding the course github, will result in a 0 for the assignment and referral to the Office of Student Conduct.



#include "app.h"
#include "stdlib.h"
#include "../grass/process.h"

// FUNCTION THAT PASSES IN THE PID AND PRIORITY FROM THE COMMAND LINE AND SENDS THEM TO THE SERVER
int main (int argc, char** argv){
    int pid;
    int prio;

    if(argc != 3)
    {
        // printf("Usage: setprio [pid] [prio]\n");
        return 1;
    }

    pid  = atoi(argv[1]);
    prio = atoi(argv[2]);
    // printf("Setting pid %d to prio %d\n", pid, prio);
    setprio(pid, prio);

    return 0;
}
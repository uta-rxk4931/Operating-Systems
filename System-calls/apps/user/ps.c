#include "app.h"
#include "../grass/process.h"

int main(int argc, char** argv) {

    int i;
    struct process * process_table = grass->proc_get_proc_set();
    
    printf("PID\tSTATUS\tPRIORITY CREATED\tCTX\n");
    for( i = 0; i < MAX_NPROCESS; i++ )
    {
      if( process_table[i].pid, process_table[i].status )
      {
        printf("%d\t%d\t%d\t %lld\t%d\n", process_table[i].pid, process_table[i].status, process_table[i].prio, process_table[i].created, process_table[i].context);
      }
    }

    return 0;
}
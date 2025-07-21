#include "app.h"
#include "../grass/process.h"

int main(int argc, char** argv) {

    int i;
    struct process * process_table = grass->proc_get_proc_set();
    
    for( i = 0; i < MAX_NPROCESS; i++ )
    {
      if( process_table[i].pid > 4 )
      {
        kill(process_table[i].pid);
      }
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "memalloc.h"

#define POOL_SIZE 1000000


void allocate_memory(int requested_size, int num_allocations) {
    
    for (int i = 0; i < num_allocations; i++) 
    {
        void* ptr1 = malloc(requested_size);

        if (ptr1 == NULL) 
        {
            fprintf(stderr, "Memory allocation failed for size %d (ptr1).\n",requested_size );
            break;
        } 
        free(ptr1); 
            
        if(i % 8==0)
        {
            void* ptr2=malloc(requested_size/2);
            if(ptr2 == NULL)
            {
                fprintf(stderr, "SMALL Memory allocation failed for size %d (ptr2).\n",requested_size/2 );
                break;
            }
            free(ptr2); 
                
        }
        else if(i % 9==0)
        {
            void* ptr3=malloc(requested_size * 2);
            if(ptr3 == NULL)
            {
                fprintf(stderr, "BIG Memory allocation failed for size %d (ptr3).\n",requested_size*2 );
                break;
            }
            free(ptr3);         
        }   
    }

}


int main() {
    struct timeval begin;
    struct timeval end;

    int test_size[]={10,100,200,500,800,1000,5000,10000,100000};
    //gettimeofday(&begin, NULL);
    
    for(int i=0;i< sizeof(test_size) / sizeof(test_size[0]); i++)
    {
         int allocation_size=test_size[i];
        gettimeofday(&begin, NULL);
     

        allocate_memory(allocation_size,5000);
 

        gettimeofday(&end, NULL);

        double duration=((end.tv_sec * 1000000)+ end.tv_usec)-((begin.tv_sec * 1000000) + begin.tv_usec);
        printf("malloc() -> Allocation Size: %d, Duration: %f microseconds\n",allocation_size,duration);

     
    }
    //gettimeofday(&end, NULL);
    //double duration=((end.tv_sec * 1000000)+ end.tv_usec)-((begin.tv_sec * 1000000) + begin.tv_usec);
    //printf("Duration: %f microseconds\n",duration);
  return 0;  
}


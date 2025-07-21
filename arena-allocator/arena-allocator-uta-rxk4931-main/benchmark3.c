#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "memalloc.h"

#define POOL_SIZE 1000000

// This function allocates and deallocates memory for various allocation sizes using the specified algorithm.
void allocate_memory(int requested_size, int num_allocations,enum ALGORITHM algorithm) {

    // Initialize memory pool with the specified size and algorithm
    if (memalloc_init(POOL_SIZE,algorithm) != 0) 
    { 
        fprintf(stderr, "Memory initialization failed.\n");
    }

    // Loop to perform memory allocations and deallocations.
    for (int i = 0; i < num_allocations; i++) {
        //Allocate memory of the specified size
        void* ptr1 = memalloc_alloc(requested_size);

        if (ptr1 == NULL) 
        {
            fprintf(stderr, "Memory allocation failed for size %d (ptr1).\n",requested_size );
            break;
        } 
        // Free the allocated memory
        memalloc_free(ptr1); 

        //Conditionally allocate smaller memory chunks at every eigth iteration   
        if(i % 8==0)
        {
            void* ptr2=memalloc_alloc(requested_size/2);
            if(ptr2 == NULL)
            {
                fprintf(stderr, "SMALL Memory allocation failed for size %d (ptr2).\n",requested_size/2 );
                break;
            }
            memalloc_free(ptr2); 
                
        }
        else if(i % 9==0)//Conditionally allocate larger memory chunks at every ninth iteration
        {
            void* ptr3=memalloc_alloc(requested_size * 2);
            if(ptr3 == NULL)
            {
                fprintf(stderr, "BIG Memory allocation failed for size %d (ptr3).\n",requested_size*2 );
                break;
            }
            memalloc_free(ptr3); 
                
        }   
    }

}


int main() {
    struct timeval begin;
    struct timeval end;
    // An array of test allocation sizes for performance evaluation
    int test_size[]={10,100,200,500,800,1000,5000,10000,100000};

    //Iterate through the test allocation sizes
    for(int i=0;i< sizeof(test_size) / sizeof(test_size[0]); i++)
    {
        int allocation_size=test_size[i];
        gettimeofday(&begin, NULL);
     
        //Call the allocate_memory function with the specified allocation size and algorithm
        allocate_memory(allocation_size,5000,WORST_FIT);
 

        gettimeofday(&end, NULL);

        double duration=((end.tv_sec * 1000000)+ end.tv_usec)-((begin.tv_sec * 1000000) + begin.tv_usec);
        printf("WORST_FIT -> Allocation Size: %d, Duration: %f microseconds\n",allocation_size,duration);
        //printf("%f \n",duration);
    }
  return 0;  
}

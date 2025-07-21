// The MIT License (MIT)
// 
// Copyright (c) 2021, 2022, 2023 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software. (RmFsbCAyMDIz)
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES UTA OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "memalloc.h"
#include<stdio.h>
#include<stdlib.h>


static struct Node ledger[MAX_ALLOCATIONS];
void* allocated_arena=NULL;
static int last = 0;
enum ALGORITHM current_algo;

int memalloc_init( size_t size, enum ALGORITHM algorithm )
{
  current_algo = algorithm;
  if (size < 0) 
  {
    return -1; //Invalid since size is less than zero 
  }

  size = ALIGN4(size);

  allocated_arena= malloc(size);

  //Setting the deafault values 
  for(int i=0;i<MAX_ALLOCATIONS;i++) 
  {
    ledger[i].arena=NULL;
    ledger[i].in_use=0;
    ledger[i].size=0;
    ledger[i].previous= -1;
    ledger[i].next= -1;
  }
  //after allocation values
   ledger[0].arena=allocated_arena;
   ledger[0].in_use=1;
   ledger[0].size=size;
   ledger[0].type = H;
   last=0;

  if( allocated_arena==NULL)
  {
    return -1;
  }

  return 0;
}

void memalloc_destroy( )
{    
      if(allocated_arena==NULL)
      {
        free(allocated_arena); //releases the arena
      }
      //setting back the deafault values
      ledger[0].arena=NULL;
      ledger[0].in_use=0;
      ledger[0].size=0;
      ledger[0].previous= -1;
      ledger[0].next= -1;
      return; 
}

int getNode(int current_node, size_t requested_size)
{
  int new_hole = -1;
  // Search for an available free space "hole" in the ledger
  for(int i=0; i<MAX_ALLOCATIONS; i++)
  {
    if(ledger[i].in_use == 0)
    {
      new_hole = i;
      break;
    }
  }
  // If not available free space is found, return -1 (allocation failed)
  if(new_hole == -1){ return -1;}
  
  // Set values for the new allocation
  ledger[new_hole].size = ledger[current_node].size - requested_size;  // Calculate leftover size
  ledger[new_hole].type = H; // Adjust the arena pointer
  ledger[new_hole].arena = (void*)((long long int )ledger[current_node].arena + requested_size); 
  ledger[new_hole].in_use = 1; // Mark the new hole as in use
  ledger[new_hole].next = ledger[current_node].next; // Set the next pointer
  
   // Update the previous pointer of the next allocation (if it exists)
  if(ledger[current_node].next != -1)
  {
    ledger[ledger[current_node].next].previous = new_hole;
  }
  // Set the previous pointer of the new hole
  ledger[new_hole].previous = current_node;
  // Update the current allocation size
  ledger[current_node].size = requested_size; 
  // Update the next pointer
  ledger[current_node].next = new_hole; 
  // Set the type of the current allocation to allocated(Process)
  ledger[current_node].type = P; 

  
  // Return the current node(index) of the current allocation
  return current_node;
}


void * memalloc_alloc( size_t size )
{
  size_t requested_size = ALIGN4( size );
  int current_node;
  int found = -1, best_fit,worst_fit;
  switch (current_algo) 
  {
    case FIRST_FIT:
        // Start from the beginning of the ledger
        current_node = 0; 
        // Initialize a variable to keep track of the found allocation
        found = -1; 

        // Iterate through the ledger until a suitable hole is found or the end of the ledger is reached
        while (current_node != -1) 
        {
          
          if (ledger[current_node].type == H && ledger[current_node].size >= requested_size) 
          {
            // Found a hole that is both of the right type and size
            found = current_node;
            break;
          }

          // Keep track of the previous node 
          last = current_node;
          // Move to the next node
          current_node = ledger[current_node].next;
        }
        
        // If no suitable hole was found, return NULL to indicate allocation failure
        if(found == -1) 
        {
          return NULL;
        }
        // If the hole is larger than the requested size, split it into two allocations
        if (ledger[current_node].size - requested_size > 0)
        {   
          
          int ret = getNode(current_node, requested_size);
          if(ret == -1) 
          {
            // If splitting the allocation fails, return NULL
            return NULL;
          }
        }
        
        // Mark the allocation as Process (P) allocated
        ledger[current_node].type = P;    
        
        // Return a pointer to the allocated memory
        return ledger[current_node].arena; 
    break;
          

    case BEST_FIT:
        // Start from the first allocation in the ledger
        current_node = 0;
        best_fit = -1;
        // Iterate through the ledger to find the best-fit hole    
        while (current_node != -1) 
        {
          // Check if the current node is a hole (type H) and if it's large enough for the requested size.
          if (ledger[current_node].type == H && ledger[current_node].size >= requested_size) 
          {
            if(best_fit == -1)
            {
               // If this is the first suitable hole encountered, set it as the best fit.
              best_fit = current_node;
            }
            if (ledger[current_node].size < ledger[best_fit].size) 
            {
              // If the current hole is smaller than the previously found best fit, update the best fit.
              best_fit = current_node;
            }
          }

          last = current_node;
          // Move to the next node in the ledger.
          current_node = ledger[current_node].next;
        }

        if(best_fit == -1) 
        {
          return NULL;
        }
        // If the hole is larger than the requested size, split it into two allocations   
        if (ledger[best_fit].size - requested_size > 0)
        {   

          int ret = getNode(best_fit, requested_size);
          if(ret == -1) 
          {
            return NULL;
          }
        }

        ledger[best_fit].type = P;    
          
        return ledger[best_fit].arena;
    break;


    case WORST_FIT:
        // Start from the first allocation in the ledger
        current_node = 0;
        worst_fit = -1;
    
        while (current_node != -1) 
        {
          // Check if the current node is a hole (type H) and if it's large enough for the requested size.
          if (ledger[current_node].type == H && ledger[current_node].size >= requested_size) 
          { 
            // If this is the first suitable hole encountered, set it as the worst fit.
            if (worst_fit == -1) 
            {
              worst_fit = current_node;
            }

            if (ledger[current_node].size > ledger[worst_fit].size)
            {
              // If the current hole is larger than the previously found worst fit, update the worst fit.
              worst_fit = current_node;
            }
            break;
          }

          last = current_node;
          // Move to the next node in the ledger.
          current_node = ledger[current_node].next;
        }

        if (worst_fit == -1) 
        {
          return NULL;
        }
        // If the hole is larger than the requested size, split it into two allocations 
        if (ledger[worst_fit].size - requested_size > 0) 
        {
          int ret = getNode(worst_fit, requested_size);
          if (ret == -1)
          {
            return NULL;
          }
        }
    
        ledger[worst_fit].type = P;
        return ledger[worst_fit].arena;

    break;


    case NEXT_FIT: 

        found = -1;
        // Start searching from the last allocation
        current_node=last;
        // If there is no last allocation, start the search from the beginning
        if(current_node == -1) 
        {
          current_node=0;
        }
        
        int start_node=current_node;
        do 
        {

          if (ledger[current_node].type == H && ledger[current_node].size >= requested_size) 
          {
            // Check if the current node is a hole (type H) and if it's large enough for the requested size
            // Store the current node of the found hole
            found = current_node; 
            break;
          }

          last = current_node;
          //Move to the next node in the ledger
          current_node = ledger[current_node].next;
          
          //if the current node has circled back to the starting node  
          //means that no suitable available memory slots found.
          if(current_node==start_node) 
          {
            current_node=0; // start searching from the beginning of the ledger 
          }
          // initiates a cyclic search starting from where it left off and looping back to the beginning if needed
        } while(current_node !=start_node); 
        

        if(found == -1) 
        {
          return NULL;
        }
        // If the hole is larger than the requested size, split it into two allocations 
        if (ledger[current_node].size - requested_size > 0)
        {   
          int ret = getNode(current_node, requested_size);
          if(ret == -1) 
          {
            return NULL;
          }
        }
        
        ledger[current_node].type = P; 

        return ledger[current_node].arena; 
              
    break;
          
    default: 
            printf("Invalid Algorithm");
            break;
  }
  return NULL;   
}


void memalloc_free( void * ptr )
{
  int current_to_free = -1;
  int i = 0;
  //looping through the ledger
  while( i != -1 ) 
  {
    if(ledger[i].arena == ptr) 
    {
      current_to_free = i;
      break;
    }
    i = ledger[i].next;
  }

  if(current_to_free ==-1)
  {
    fprintf(stderr,"ERROR...given pointer does not correspond to any allocated block");
    return;
  }
 
  //Combine with previous hole if present
  if(ledger[ledger[current_to_free].previous].type == H)
  {
    ledger[ledger[current_to_free].previous].size += ledger[current_to_free].size + sizeof(struct Node);
    ledger[ledger[current_to_free].previous].next = ledger[current_to_free].next;
    current_to_free=ledger[current_to_free].previous;
  }

  //Combine with next hole if present
  if(ledger[ledger[current_to_free].next].type == H)
  {
    ledger[current_to_free].size += ledger[ledger[current_to_free].next].size + sizeof(struct Node);
    ledger[current_to_free].next = ledger[ledger[current_to_free].next].next;
  }
  //return the block back to arena
  ledger[current_to_free].type=H;
}



int memalloc_size( )
{
  int number_of_nodes = 0;
  // Initialize a variable to track the current node
  int current_i=0;

  while(current_i<MAX_ALLOCATIONS)
  {
    // Check if the current node index is at the end of the ledger
    if(current_i == -1) 
    {
      break;
    }
   
     // Check if the current node is in use
    if(ledger[current_i].in_use==1) 
    {
      number_of_nodes++; // Increment the counter for found nodes
    }
    
    // Move to the next node 
    current_i=ledger[current_i].next;
  }
  // Return the total number of nodes  in use
  return number_of_nodes;
}



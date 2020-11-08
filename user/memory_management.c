#include "memory_management.h"



static header *freep = 0; //global pointer to start of the heap

header * find_block(header **last, uint nunits){

    header * b = freep; //start at the beginning of the list
    //int flag = 0;

    
    while(b!= 0 && !(b->s.free && b->s.size >= nunits) ){ 
        //printf("INSIDE LOOP\n");

        *last = b;
        b = b->s.next;
        

    }
    return b; 

}

void split_block(header * b, uint nunits){
    // b -> new
    header * new;
    new = b + nunits;

    //new->s.size = b->s.size - nunits - sizeof(header);
    new->s.size = b->s.size - nunits;
    new->s.next = b->s.next;
    new->s.prev = b;
    new->s.free = 1;

    b->s.size = nunits;
    b->s.next = new;

    //if we're on the last header of the list and its size is bigger than 4096 (but less than what was requested):
    //we'll give some memory back to the OS, making the last header's size 4096 
    if((new->s.next == 0) && (new->s.size > 4096)){
        //printf("LAST MEMORY SIZE BEFORE GIVING BACK TO OS: %d\n", new->s.size ); 

        sbrk( -((new->s.size-4096) * sizeof(header)) );
        new->s.size -= new->s.size-4096; 

       //printf("LAST MEMORY SIZE AFTER GIVING BACK TO OS: %d\n", new->s.size ); 

    }


}

header * extend_heap(header * last, uint nunits){


    void *freemem;
    header * b;

    //nunits has what was asked
    //nunits has what was actually allocated
    uint nunits_2;

    if (nunits < 4096) {
        nunits_2 = 4096;
    }
    else{
        nunits_2 = nunits;
    }

    freemem= sbrk(0);
    //freemem = sbrk(nunits_2 * sizeof(header)); 
    //don't have full understanding of how sbrk() is working, might affect my performance
    //I'm assuming that from 4096 onwards it allocates exacly the memory I need, and not full pages (multiples of 4096)

    if(sbrk(nunits_2 * sizeof(header)) == ((void*)-1)){

        return 0;
    }

    //creating new header
    b = (header *) freemem;
    b->s.size = nunits_2;
    b->s.next = 0; 
    b->s.free = 0; // this will be the header for what's asked from the user, so won't be free


    //this is not the first header
    if( last != 0){
        last->s.next = b;
        b->s.prev = last;
    }else{ //this is the first header

        
        b->s.prev = 0; 
    }

    if(b->s.size > nunits ){
        split_block(b, nunits);

    }

    return b; 
}

void* _malloc(uint nbytes){

    uint nunits;
    header * b, *last;

    //Calculate the number of memory units needed to provide at least nbytes of memory
    //nunits INCLUDES the unit for the header already
    nunits = ((nbytes + sizeof(header) - 1)/sizeof(header)) + 1;  
   


    //base exists? ie, heap initialized?
    if(freep != 0){

        //first find a block, first fit
        last = freep;
        b = find_block(&last, nunits);
        //printf("MEMORY FOUND: %d\n", b->s.size ); //gives 4094

        if(b){

            //can we split?
            if(b->s.size > nunits ){
                split_block(b, nunits);
            }
            //printf("MEMORY SPLIT: %d\n", b->s.size ); 
            //printf("NEXT MEMORY: %d\n", b->s.next->s.size ); 
            b->s.free = 0;
            
        }else{

            //No fitting block, extend the heap 
            b = extend_heap(last, nunits);
            if(!b){
                return 0;
            }
        }
        
    }else{

        //printf("INITIALIZING THE HEAP\n");
        // case: no free list yet exists; we have to initialize.
        b = extend_heap(0,nunits);
        if(!b){
            return 0;
        }
       

        freep = b;
    }

    //printf("ASKED MEMORY SIZE: %d\n", nunits);
    //printf("RETURNED MEMORY SIZE: %d\n", b->s.size ); //2
    //printf("RETURNED MEMORY STATUS: %d\n", b->s.free ); //0
    //printf("RETURNED NUMBER OF BYTES: %d\n", (b->s.size * (sizeof(header) +1)) - sizeof(header) +1);
    return (void *) (b + 1);

}

void _free(void *ap){


//Step 1: Mark block as free
//Step 2: Coalesce adjacent free blocks
//Step 3: Insert free block into the free list; 

    header *c;

    // Find address of block header for the data to be inserted
    c = ((header *) ap) - 1;
    c->s.free = 1;
    

    header * next  =  c->s.next;
    header * prev = c->s.prev;

    //if RIGHT is free MERGE
    if((next != 0) && (next->s.free == 1)){

        if(next->s.next != 0){
            next->s.next->s.prev = c;
        }
        c->s.size += next->s.size;
        c->s.next = next->s.next;
    }

    //if LEFT is free MERGE
    if((prev != 0) && (prev->s.free == 1)){

        prev->s.size += c->s.size;
        prev->s.next = c->s.next;

        if(c->s.next != 0){
            c->s.next->s.prev = prev;
        }

        c = prev;
    }


}
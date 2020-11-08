#include "kernel/types.h"
//#include "kernel/stat.h"
#include "user/user.h"
//#include "kernel/param.h"


//The header file memory_management.h should contain the following function definitions as well as any structs that you may choose to implement.

typedef long Align;

union Header {
  struct {
    union Header *next;
    union Header *prev;
    uint size;
    int free;
  } s;
  Align x;
};

typedef union Header header;


header * find_block(header **last, uint nunits);
void split_block(header * b, uint nunits);
header * extend_heap(header * last, uint nunits);
void* _malloc(uint nbytes);

void _free(void *ap);



/*
union header {
  struct {
    uint size; //keeping track of sizes, and using last bit as free/not free, we know that sizes always end up in 0 due to alignment
    //int free; //checking if it's free 
  } s;
  Align x;
};
*/

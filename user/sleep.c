#include "user/user.h"
#include "kernel/types.h"


int main(int argc, char *argv[]) {
  // insert  code below to print "Hello world" to the console
  // you can use the printf function as you would normally.


/*Do something*/



    int number = 30; 

    printf("...\n");
    if(argc >2 ) {
        printf("You need to provide only one argument\n");
    }else if(argc == 2){

        sleep(atoi(argv[1]));

    }else{
        sleep(number);
    }


    printf("...\n");



  // Terminate the process
  exit(0);
}
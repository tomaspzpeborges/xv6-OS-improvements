#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"
#include <sys/types.h>




int main(int argc, char *argv[]) {
  // insert code below to print "Hello world" to the console
  // you can use the printf function as you would normally.


/*Do something*/

    
    int n=0;
    int number = 0;


    if( argc == 1){ //no args
        printf("Provide arguments filename and n (no. of lines), in this order\n");
    }
    if(argc == 2){ //just number of lines specified, so we'll read stdin inputs


        n = atoi(argv[1]);
        if(n == 0){
            printf("First argument after exec name should be the number of lines to read");
        }


        while (number < n){

            char str[100]; 
            gets(str, 100);
            printf("%s", str);
            number++;
        }

    }
    if(argc == 3 ) { //number of lines + filename


        n = atoi(argv[1]);
        if(n == 0){
            printf("First argument after exec name should be the number of lines to read");
        }

        int filedesc = open(argv[2], O_RDONLY);
        if(filedesc < 0) {

            printf("File could not be opened\n"); 
            exit(1);

        }


        while (number < n){

            char c;
            read(filedesc,&c,1);
            printf("%c", c);
            if(c == '\n'){
                number++; //one more line
            }
        }
           
        close(filedesc);

    
    } if(argc == 4){
        printf("Too many arguments\n");
    }





  // Terminate the process
  exit(0);
}
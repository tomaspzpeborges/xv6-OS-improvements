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



void createForks( int n ){

    if(n == 0) return;

    int p[2]; 
    char str[20];
    pipe(p);

    if(fork() == 0) {//child process writes 


        write(p[1], "Hello from child ", 19);
        createForks( n-1);


    } else { //parent process read
        wait(0);//writings were getting messed up without wait(0)
        read(p[0], str, 19);
        printf("%s %d\n", str, n-1);
    } 

}

int main(int argc, char *argv[]) {

  

    if(argc != 2){
        printf("Please only provide 1 argument: number of forks\n");
    }else{

        printf("Hello from parent\n");//first process is the main one
        createForks(atoi(argv[1])-1);//so there are n-1 processes to be created

    }

    exit(0);

}



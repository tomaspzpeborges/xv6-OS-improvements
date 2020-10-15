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


 
    if(argc != 2){ 
        printf("Please only provide 1 argument: number of times the ball goes through the pipes\n");

    }else{ //second argument is the number of times the "ball" goes back and forth
    
        int  p2c[2], c2p[2];
        int number =atoi(argv[1])-1; //first throw is made before the loops

        pipe(p2c);    //parent to child pipe
        pipe(c2p);    //child to parent pipe

        printf("Parent throws the ball... %d\n", number+1);
        write(p2c[1], &number, sizeof(int));


        if (fork () == 0 ) { //child process

            while(number > 1)
            {
                //printf("in child process\n");
                read(p2c[0], &number, sizeof(int));
                printf("CHILD received number: %d, process: %d\n", number, getpid());
                number = number - 1;
                write(c2p[1], &number, sizeof(int));
            } 
            close(p2c[0]); close(p2c[1]);
            close(c2p[0]); close(c2p[1]);
            exit(0);

        } else {

            while(number > 1) {
            // printf("in parent process\n");
                read(c2p[0], &number, sizeof(int));
                printf("PARENT received number: %d, process: %d\n", number, getpid());
                number = number - 1;
                write(p2c[1], &number, sizeof(int));
            }
            close(p2c[0]); close(p2c[1]);
            close(c2p[0]); close(c2p[1]);
            wait(0);

        }

    } 

    exit(0);

}

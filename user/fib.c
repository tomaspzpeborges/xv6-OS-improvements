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
  // insert  code below to print "Hello world" to the console
  // you can use the printf function as you would normally.


/*Do something*/

    int first = 0;
    int second = 1;
    int aux = 0;
    int number = 5; 
    int filedesc;

    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe 
  
    char fixed_str[] = "fib_"; 
    pid_t p; 

    char *input_str = "5"; 

    if(argc >2 ) {
        printf("You need to provide only one argument\n");
    }else if(argc == 2){

        input_str = argv[1];
        number = atoi(argv[1]);

    }



    if (pipe(fd1)==-1) 
    { 
        printf("Pipe Failed\n" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        printf("Pipe Failed\n" ); 
        return 1; 
    } 

    p = fork(); 

    if (p < 0) 
    { 
        printf("Pipe Failed\n" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        char concat_str[100]; 
  
        close(fd1[0]);  // Close reading end of first pipe 
  
        // Write input string and close writing end of first 
        // pipe. 
        write(fd1[1], fixed_str, strlen(fixed_str)+1); 
        close(fd1[1]); 
  
        // Wait for child to send a string 
        wait(0); 
  
        close(fd2[1]); // Close writing end of second pipe 
  
        // Read string from child, print it and close 
        // reading end. 
        read(fd2[0], concat_str, 100); 
        printf("File name: %s\n", concat_str); 
        filedesc = open(concat_str, O_CREATE|O_WRONLY);

        close(fd2[0]); 
    } 
     else
    { 
        close(fd1[1]);  // Close writing end of first pipe 
  
        // Read a string using first pipe 
        char concat_str[100]; 
        read(fd1[0], concat_str, 100); 
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(input_str); i++) 
            concat_str[k++] = input_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[0]); 
  
        // Write concatenated string and close writing end 
        write(fd2[1], concat_str, strlen(concat_str)+1); 
        close(fd2[1]); 
  
        exit(0); 
    } 


 


    printf("...\n");

    printf("%d\n", first);
   // fprintf(fptr,"%d\n", first); 
    fprintf(filedesc,"%d\n",first );
    printf("%d\n", second);
   // fprintf(fptr,"%d\n", second); 
    fprintf(filedesc,"%d\n",second );



    for (int i = 2; i < number; i++){

        aux = second;
        second = first + second;
        first = aux;

        printf("%d\n", second);
       // fprintf(fptr,"%d\n", second); 
        fprintf(filedesc,"%d\n",second );

        
    }


    printf("...\n");
    close(filedesc);



  // Terminate the process
  exit(0);
}
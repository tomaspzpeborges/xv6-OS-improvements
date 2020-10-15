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
    

    if( argc == 1){ //no arguments, reading line from console

            printf("Write a line of text: \n");
            char str[300]; //reading the line
            gets(str, 300);

            char *strs[100];//max 100 words
            int gen_index = 0;
            char buff[25] = "";
            int index = 0;

            //FOUND SOME FAIRLY BIG WHEN I WAS RE USING THIS LOOP, MY CORRECTIONS WERE IMPLEMENTED IN mysh.c
            for(int i =0; i <sizeof(str); i++){ 
                
                if ( (str[i] == ' ') || (str[i] == '\t') ){

                    //copying word from buffer to array
                    strs[gen_index] = (char*) malloc( sizeof(buff));
                    strcpy(strs[gen_index], buff);

                    //resetting buffer
                    index = 0;
                    memset(buff,0,sizeof(buff));


                    gen_index++;
                    if(gen_index == 100) break;

                    continue;

                }else{

                    buff[index] = str[i];
                    index++;

                    if(index == 25) { //word has to end

                        //copying word from buffer to array
                        strs[gen_index] = (char*) malloc( sizeof(buff));
                        strcpy(strs[gen_index], buff);

                        //resetting buffer
                        index = 0;
                        memset(buff,0,sizeof(buff));


                        gen_index++;
                        if(gen_index == 100) break;
                    }

                 }

            }

            //sorting...
            for (int i=0; i < gen_index; i++){

                for(int z=i+1; z<gen_index; z++){

                    if(strcmp(*(strs+i),*(strs+z))>0){
                        char *temp = *(strs+i);
                        *(strs+i) = *(strs+z);
                        *(strs+z) = temp;
                    }
            
                }
            }

            for(int i = 0; i < gen_index; i++){

                printf("%s ", strs[i] );
                free(strs[i]);

            }

         
        
    }
    if(argc == 2){ //filename specified


        int filedesc = open(argv[1], O_RDONLY);
        if(filedesc < 0) {

            printf("File could not be opened\n"); 
            exit(1);

        }

        //transform text in arrays
        char *strs[100];//max 100 words
        int gen_index = 0;
        char c;
        char buff[25] = "";
        int index = 0;

        while ( read(filedesc,&c,1) != 0 ){ //it returns 0 at the end of file

            //printf("%s %d ", buff, gen_index);
            
            if ((c == '\n') || (c == ' ' || c == '\t')){

                //copying word from buffer to array
                strs[gen_index] = (char*) malloc( sizeof(buff));
                strcpy(strs[gen_index], buff);

                //resetting buffer
                index = 0;
                memset(buff,0,sizeof(buff));


                gen_index++;
                if(gen_index == 100) break;

                continue;
               
            }else{

                buff[index] = c;
                index++;

                if(index == 25) { //word has to end

                    //copying word from buffer to array
                    strs[gen_index] = (char*) malloc( sizeof(buff));
                    strcpy(strs[gen_index], buff);

                    //resetting buffer
                    index = 0;
                    memset(buff,0,sizeof(buff));


                    gen_index++;
                    if(gen_index == 100) break;
                }

            }
        }

        close(filedesc);


        //sorting...
        for (int i=0; i < gen_index; i++){

            for(int z=i+1; z<gen_index; z++){

                if(strcmp(*(strs+i),*(strs+z))>0){
                    char *temp = *(strs+i);
                    *(strs+i) = *(strs+z);
                    *(strs+z) = temp;
                }
        
            }
        }

        for(int i = 0; i < gen_index; i++){

            printf("%s ", strs[i] );
            free(strs[i]);
        }



    }if(argc == 3){
        printf("Too many arguments\n");
    }



  // Terminate the process
  exit(0);
}

/*
char * arrayFromText(){


}
*/




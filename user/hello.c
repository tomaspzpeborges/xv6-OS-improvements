#include "user/user.h"
#include "kernel/types.h"


int main(int argc, char *argv[]) {
  // insert  code below to print "Hello world" to the console
  // you can use the printf function as you would normally

    char name[100];
    //printf("hello. what is your name ");
    //scanf("%s", &name);  - deprecated
    gets(name,100);
    printf("hello %s", name);

  // Terminate the process
  exit(0);
}
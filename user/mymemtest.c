#include "memory_management.c"


int
main(char *s)
{
  printf("testing...\n");

  void *m1, *m2;
  int pid;

  if((pid = fork()) == 0){
    m1 = 0;
    while((m2 = _malloc(10001)) != 0){
      //printf("infinite loop\n");
      *(char**)m2 = m1;
      m1 = m2;
    }
    while(m1){
      m2 = *(char**)m1;
      _free(m1);
      m1 = m2;
    }
    m1 = _malloc(1024*20);
    if(m1 == 0){
      printf("couldn't allocate mem?!!\n", s);
      exit(1);
    }
    _free(m1);
    printf("if you see this, it's probably worked\n", s);
    exit(0);
  } else {
    int xstatus;
    wait(&xstatus);
    if(xstatus == -1){
      // probably page fault, so might be lazy lab,
      // so OK.
      exit(0);
    }
    exit(xstatus);
  }
}
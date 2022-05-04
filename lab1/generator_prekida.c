#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

int pid = 0;
int sig[] = {SIGFPE, SIGHUP, SIGILL, SIGSYS};

void prekidna_rutina(int sig){
   kill(pid, SIGKILL);
   exit(0);
}

int main(int argc, char *argv[]){
   
   pid=atoi(argv[1]);
   sigset(SIGINT, prekidna_rutina);
   srand((unsigned) time(NULL));

   int t;

   while(1){
      sleep(rand() %(5 - 3 + 1) + 3);
      t = rand() % (3 - 0 + 1) + 0;
      printf(" Šaljem %d \n", t);
      kill(pid, sig[t]);
   }
   return 0;
}
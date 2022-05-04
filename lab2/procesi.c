#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>


int *A;
int Id;


void povecajA(int x) {
    for(int i = 0; i < x; i++) {
        (*A)++;
    }
}


int main(int argc, char *argv[]) {

    int i;                                          
    int N = atoi(argv[1]), M = atoi(argv[2]);        // uzima zadane argument iz terminala

    Id = shmget(IPC_PRIVATE, sizeof(int), 0600);     // stvara novi segment 

    if (Id == -1) {                                  // u slučaju da nije uspio kreirati segment završava proces
        exit(1);
    }    

    A = (int *) shmat(Id, NULL, 0);                 // povezuje kreirani segment na adresni prostor koji će jezgra odabrati
    *A = 0;                                  

    // kreiranje N procesa
    for (i = 0; i < N; i++) {                        
        switch (fork()) {
            case -1:
                printf("Nije kreiran proces. \n");
                break;
            case 0:                                  // posao procesa dijeteta 
                povecajA(M);
                exit(0);
            default:                                // ukoliko nije uspijelo kreiranje djeteta izlazi iz petlje
                break;
        }
    }
    while (i--) {                                   // čeka da dijete završi 
        wait(NULL);
    }

    printf("A = %d \n", *A);                         // ispis rezultata

    return 0;
}
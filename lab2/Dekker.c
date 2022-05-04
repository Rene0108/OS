#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

#define BROJ_PROCESA 2

int *A;
atomic_int *PRAVO;
atomic_int *ZASTAVICA;


void ulaz_u_KO(int i, int j){               // ulaz u kriticni odsjecak;
	ZASTAVICA[i] = 1;
	while(ZASTAVICA[j] != 0 ) {
		if(*PRAVO == j) {
			ZASTAVICA[i] = 0;
			while(*PRAVO == j);
			ZASTAVICA[i] = 1;
		}
	}
}


void izlaz_iz_KO(int i, int j) {            // izlaz iz kriticnog odsjedcka;
	*PRAVO = j;
	ZASTAVICA[i] = 0;
}


void proces(int i, int M) {
    for(int j = 0; j < M; j++) {
        ulaz_u_KO(i, 1-i);
        (*A)++;
        izlaz_iz_KO(i, 1-i);
    }
}


int main(int argc, char *argv[]) {
 
    int i;                                          
    int M = atoi(argv[1]);        // uzima zadani argument iz terminala

    int *memorija;

    int mem_Id;
    mem_Id = shmget(IPC_PRIVATE, sizeof(int) * (BROJ_PROCESA + 2), 0600);      // kreiranje zajedničkog prostora
    memorija = shmat(mem_Id, NULL, 0);

    if (mem_Id == -1) {                                  // u slučaju da nije uspio kreirati segment završava proces
        fprintf(stderr, "Kreiranje zajedničkog segmenta neuspješno. \n");
        exit(1);
    }    

    A = memorija;                                        // povezivanje kreiranog segmenta na adresni prostor i inicijalizacija
    *A = 0; 

    PRAVO = (atomic_int *) A + 1;
    *PRAVO = 0;

    ZASTAVICA = (atomic_int *) PRAVO + 1;
    ZASTAVICA[0] = 0;
    ZASTAVICA[1] = 0;                      

    // kreiranje 2 procesa
    for (i = 0; i < BROJ_PROCESA; i++) {                        
        if (fork() == 0) {
            proces(i, M);
            exit(0);
        }
    }
    for (i = 0; i < BROJ_PROCESA + 2; i++) {         // hvatanje djece
        wait(NULL);
    }

    printf("A = %d \n", *A);                         // ispis rezultata

    (void) shmdt((char *) A);
    (void) shmdt((char *) PRAVO);
    (void) shmdt((char *) ZASTAVICA);
    (void) shmctl(mem_Id, IPC_RMID, NULL);

    return 0;
}
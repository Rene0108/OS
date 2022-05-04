#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>


int A, N, M;


void *funkcija(void *m) {
    for (int i = 0; i < M; i++) {
        A++;
    }
    return NULL;
}

int main (int argc, char *argv[]) {
    
    A = 0;

    N = atoi(argv[1]);
    M = atoi(argv[2]);

    pthread_t t[N];

    for (int i = 0; i < N; i++) {
        if (pthread_create(&t[i], NULL, funkcija, NULL))  {
            printf("Kreiranje dretve nije uspjelo. \n");
            exit(1);
        }
    }
    for (int j = 0; j < N; j++) {
        pthread_join (t[j], NULL);
    }

    printf("A = %d \n", A);

    return 0;
}

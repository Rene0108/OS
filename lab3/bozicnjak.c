#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdatomic.h>
#include <time.h>

#define BROJ_SOBOVA 10

sem_t *kriticni;
sem_t *deda;
sem_t *sob;
sem_t *konzultacije;

int *br_patuljaka;
int *br_sobova;


void sobovi() {
    sleep(1);

    sem_wait(kriticni);

	  *br_sobova = *br_sobova + 1;
	  printf ("Sobova pred vratima: %d\n", *br_sobova);

	  if (*br_sobova == 10) {
        printf("-------------------------------------\n");
        printf("Ispred vrata stoji 10 sobova.\n-------------------------------------\n");
        printf("Djed Božičnjak se probudio.\n-------------------------------------\n");
        sem_post(deda);
    }
    sem_post(kriticni);
}


void patuljci() {
   
    sleep(1);

    sem_wait(kriticni);

	  *br_patuljaka = *br_patuljaka + 1;
	  printf ("Patuljaka treba pomoć: %d\n", *br_patuljaka);

	  if (*br_patuljaka == 3) {
        //sleep(1);
        printf("-------------------------------------\n");
        printf("Patuljci idu probuditi Djeda.\n-------------------------------------\n");
        sleep(1);
        printf("Djed Božičnjak je probuđen.\n");
        sem_post(deda);
    }
    sem_post(kriticni);
    sem_wait(konzultacije);
}


void sjeverni_pol() {
    printf("Dobro došli na Sjeverni pol!\nWelcome to the North Pole!\nWillkommen am Nordpol! \n-------------------------------------\n");
    while (1) {
        int vjerojatnost;
        int vjerojatnostP;

        sleep(rand() % 3 + 1);  //čekaj_slučajno_vrijeme od 1 do 3 sekunde
        srand(time(NULL));

        vjerojatnost = rand() % 100;
        vjerojatnostP = rand() % 100;

        if (*br_sobova < 10 && vjerojatnost > 50) {
            if (fork() == 0) {
                sobovi();
                exit(0);
            }
        }
        if (vjerojatnostP > 50) {
            if (fork() == 0){
                patuljci(); 
                exit(0);
            }
        }
    } 
}

void Djed_Mraz() {

    while (1){        
        sem_wait(deda);
        sem_wait(kriticni);

		    if (*br_sobova == 10 && *br_patuljaka != 0) {
            sem_post(kriticni);
            printf("-------------------------------------\n");
            printf("Djed ukrcava poklone i razvozi ih.\n-------------------------------------\n");
            sleep(3);
            sem_wait(kriticni);
            printf("Sobovi otišli na Havaje.\n-------------------------------------\n");
            sleep(4);
            printf("    GOTOV GODIŠNJI!\n-------------------------------------\n");
            *br_sobova = 0;   
            sleep(1);
        
        } else if (*br_sobova == 10 ) {
            sem_post(kriticni);
            printf("-------------------------------------\n");
			      printf ("Djed Božičnjak hrani sobove i vraća se na spavanje.\n-------------------------------------\n");
			      sleep(2);
            sem_wait(kriticni);
        }
		    
        while (*br_patuljaka >= 3) {
            sem_post(kriticni);
            printf("-------------------------------------\n");
			      printf("Djed pomaže patuljcima riješiti problem i vraća se na spavanje.\n-------------------------------------\n");
            sleep(2);
            *br_patuljaka = *br_patuljaka - 3;
            sem_wait(kriticni);

            for (int i = 0; i < 3; i++) {
                sem_post(konzultacije);
            }
		    }
        sem_post(kriticni);
	  }
}


int main() {
    int id;
    id = shmget(IPC_PRIVATE, 4 * sizeof(sem_t) + sizeof(int) * 2, 0600);
    if (id == -1) {
        printf("Nema zajedničke memorije!\n");
        exit(1);  /* greška - nema zajedničke memorije */
    }

    kriticni = shmat(id, NULL, 0);
    deda = (sem_t *) (kriticni + 1);
    konzultacije = (sem_t *) (deda + 1);
    sob = (sem_t *) (konzultacije + 1);
    shmctl(id, IPC_RMID, NULL);

    br_sobova = (int *) (sob + 1);
    br_patuljaka = (int *) (br_sobova + 1);
    //printf("%p %p\n", br_patuljaka, br_sobova);

    sem_init(kriticni, 1, 1);
    sem_init(deda, 1, 0);
    sem_init(konzultacije, 1, 0);
    //sem_init(sob, 1, 10);

    *br_patuljaka = 0;
    *br_sobova = 0;

    if (fork()==0) {
        Djed_Mraz();
    }

    if (fork()==0) {
        sjeverni_pol();
    }

    wait(NULL);
    wait(NULL);

    sem_destroy(deda);
    sem_destroy(kriticni);
    sem_destroy(sob);
    sem_destroy(konzultacije);

    shmdt(deda);
    shmdt(kriticni);
    shmdt(sob);
    shmdt(konzultacije);

    exit(0);
}

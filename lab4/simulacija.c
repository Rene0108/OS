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

int brojac = 0;
int velicina;
int pauza = 0;

int postavi_pauzu(int signal) {            // postavljanje pauze 
    pauza = 1 - pauza;
    return pauza;
}

void ispis_polja(int *pok, int size) {
    int i, pom = 0;

    /*for (i = 0; i < size; i++) {
        if(pom == 10) {
            pom = 0;
        }
        printf("%d ", pom);
        pom++;
    }
    printf("\n");*/

    printf("polje: ");

    for (i = 0; i < size; i++) {
        printf("%d ", *pok++);
    }
    printf("\n");

    return;
}

void naredbaZ(int *pok, int size) {
    int upisanih = 0;
    int dodatno = 0;

    for (int i = 0; i < velicina; i++) {
        if (*pok == -1 && upisanih < size) {
            *pok = brojac;
            upisanih++;
            pok++;
        } else {
            pok++;
            dodatno++;
        }
    }
    if (dodatno == velicina) {
        printf ("\nSpremnik je pun nema mjesta.\n\n");
    }
    brojac++;
    return;
}

void naredbaO(int *pok, int zahtjev) {
    int bilo_ih_je = 0;

    for (int i = 0; i < velicina; i++) {
        if (*pok == zahtjev) {
            *pok = -1;
            bilo_ih_je = 1;
            pok++;
        } else {
            pok++;
        }
    }
    if (bilo_ih_je == 0) {
        printf("U polju nema traženih zahtjeva.");
    }
    return;
}

int main(int argc, char *argv[]) {

    char naredba;
    velicina = atoi(argv[1]);
    int polje[velicina];
    int zahtjev;

    sigset(SIGINT, postavi_pauzu);

    for (int i = 0; i < velicina; i++) {
        polje[i] = -1;
    }

    ispis_polja(polje, velicina);

    while(1) {
        printf("Upišite oznaku naredbe: ");
        scanf(" %c", &naredba);

        srand(time(NULL));
        int prostor = (rand() % (3 + 1)) + 1;

        if (naredba == 'Z' || naredba == 'z') {
            naredbaZ(polje, prostor);
            ispis_polja(polje, velicina);

        } else if (naredba == 'O' || naredba == 'o') {
            printf("Koji zahtjev treba osloboditi?\n");
            scanf(" %d", &zahtjev);
            printf("Oslobađa se zahtjev %d !\n", zahtjev);
            naredbaO(polje, zahtjev);
            ispis_polja(polje, velicina);

        } else if (naredba == 'D' || naredba == 'd') {
            //DEFRAGMENTACIJA
            int prazno;
            for (int i = 1; i < velicina; i++) {
                prazno = 0;
                for (int j = i - 1; j >= 0; j--) {
                    if (polje[j] == -1) {
                        prazno++;
                    }
                }
                polje[i - prazno] = polje[i];
                polje[i] = -1;
            }
            printf("DEFRAGMETIRANJE...\n");
            ispis_polja(polje, velicina);
            
        } else {
            printf("Unjeli ste pogrešnu naredbu!\n");
        }

        while(pauza != 0){
            ;                   // zaustavljanje
        } 
    }	
    return 0;
}
	

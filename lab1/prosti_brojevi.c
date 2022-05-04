#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED

int pauza = 0;
long broj = 1000000001;
long zadnji = 1000000001;

void periodicki_ispis(int signal) {        // ispisuje zadnji broj koji je provjereno prost
    printf("Zadnji : %ld \n", zadnji);
    return;
}

int postavi_pauzu(int signal) {            // postavljanje pauze 
    pauza = 1 - pauza;
    return pauza;
}

void prekid(int signal) {                  // ispisuje zadnji provjereni prosti broj i izlazi iz programa uz povratnu vrijednost 0
    printf("Zadnji : %ld \n", zadnji);
    exit(0);                               // izlaz iz programa
}

int prost(unsigned long n) {

	unsigned long i, max;

	if ((n & 1) == 0) {                     // je li paran? 
		return 0;                           // ako je, vrati 0 i broj nije prost
    }
	max = sqrt(n);
	for(i = 3; i <= max; i += 2) {
		if ((n % i) == 0) {
			return 0;                       // broj nije prost
        }
    }
	return 1;                               // broj je prost
}


int main(void) {

    struct itimerval t;

    sigset(SIGALRM, periodicki_ispis);      // dodavanje funkcija signalima 
    sigset(SIGINT, postavi_pauzu);
    sigset(SIGTERM, prekid);

    // PRVI BLOK vrijeme čekanja do prvog ispisa
    t.it_value.tv_sec = 4;
    t.it_value.tv_usec = 500000;
   
    //DRUGI BLOK vrijeme čekanja do drugog ispisa
    t.it_interval.tv_sec = 8;
    t.it_interval.tv_usec = 500000;
      
    setitimer(ITIMER_REAL, &t, NULL);

    int l = 0;
      
    while(1) {
        l = prost(broj);
        if(l) {
            zadnji = broj;
        }
        broj++;
        while(pauza != 0){
            pause();                   // zaustavljanje
        }  
    }
    return 0;
} 


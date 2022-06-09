#include <stdio.h>
#include <signal.h>

// broj razina proriteta 
#define N 6         
#define number 5                                       

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

int sig[N] = { 0, SIGFPE, SIGHUP, SIGILL, SIGSYS, SIGINT };             // kreiranje polja signala koje ćemo koristiti

// ZABRANA PREKIDANJA
void zabrani_prekidanje() {
      for(int i = 0; i < 5; i++) {
            sighold(sig[i]);
      }
}

// PONOVNA DOZVOLA PREKIDANJA
void dozvoli_prekidanje() {
      for(int i = 0; i < 5; i++) {
            sigrelse(sig[i]);
      }
}

// GLAVNA OBRADA
void obrada_signala(int i) {                         
      
      int j;
     
      if(i == 1) {
            printf("-  P  -  -  -  -\n");
            for(j = 0; j < number; j++) {                    // for petlja OSIGURAVA 5 signala...to je zadano u zadatku
                  printf(" -  %d  -  -  -  - \n", j);
                  sleep(1);                                // jedan ispis = jedna sekunda
            }
      } else if(i == 2) {
            printf(" -  -  P  -  -  - \n");
            for(j = 0; j < number; j++) {
                  printf(" -  -  %d  -  -  - \n", j);
                  sleep(1);
            }
      } else if(i == 3) {
            printf(" -  -  -  P  -  - \n");
            for(j = 0; j < number; j++) {
                  printf(" -  -  -  %d  -  - \n", j);
                  sleep(1);
            } 
      } else if(i == 4) {
            printf(" -  -  -  -  P  - \n");
            for(j = 0; j < number; j++) {
                  printf(" -  -  -  -  %d  - \n", j);
                  sleep(1);
            }
      } else if(i == 5) {
            printf(" -  -  -  -  -  P \n");
            for(j = 0; j < number; j++) {
                  printf("-  -  -  -  -  %d\n",j);
                  sleep(1);
            }
      }

}

void prekidna_rutina(int sig){

      int x = -1, y, i;

      zabrani_prekidanje();

      if (sig == SIGFPE) {
            x = 1; 
            printf(" -  X  -  -  -  - \n"); 
      } else if (sig == SIGHUP) { 
            x = 2; 
            printf(" -  -  X  -  -  - \n");
      } else if (sig == SIGILL) {
            x = 3;
            printf(" -  -  -  X  -  - \n");
      } else if (sig == SIGSYS) {
            x = 4;
            printf(" -  -  -  -  X  - \n");
      } else if (sig == SIGINT) {
            x = 5;
            printf(" -  -  -  -  -  X \n");
      }

      OZNAKA_CEKANJA[x] += 1; // oznaka cekanja se ne postavlja u 1 nego se povecava za jedan kao sto je napomenuto
   
      do {
            y = 0;
            for(i = TEKUCI_PRIORITET + 1; i < N; i++) {
                  if(OZNAKA_CEKANJA[i] > 0) {
                        y = i;
                  }
            } 
            if(y > 0) {
                  OZNAKA_CEKANJA[y] = 0;
                  PRIORITET[y] = TEKUCI_PRIORITET;
                  TEKUCI_PRIORITET = y ;

                  dozvoli_prekidanje();
                  obrada_signala(x);
                  zabrani_prekidanje();

                  TEKUCI_PRIORITET = PRIORITET[y];
                  PRIORITET[y] = 0;
           }
         
      } while(y > 0);
   
      dozvoli_prekidanje();
}




int main(void) {

      sigset(sig[1], prekidna_rutina);
      sigset(sig[2], prekidna_rutina);
      sigset(sig[3], prekidna_rutina);
      sigset(sig[4], prekidna_rutina);
      sigset(sig[5], prekidna_rutina);

      printf("Proces obrade prekida, PID=%d \n", getpid());
      printf(" G 1 2 3 4 5 \n");
      printf(" ----------- \n");
      printf(" 1 - - - - - \n");
      printf(" 2 - - - - - \n");
      printf(" 3 - - - - - \n");
      printf(" 4 - - - - - \n");
      printf(" GP S1 S2 S3 S4 S5 \n");
      printf(" ----------------- \n");

      for(int i = 0; i < 15; i++){
            printf(" %d  -  -  -  -  - \n", i );
            sleep(1);
      }
   
      printf ("Zavrsio osnovni program. \n");

      return 0;
}

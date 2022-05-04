#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

char filozof[6] = {'O', 'O', 'O', 'O', 'O', 'O'};
int vilica[5] = {1, 1, 1, 1, 1};

pthread_cond_t red[5];
pthread_mutex_t monitor;

// kreiran "bespotrebni" kritični odsječak zbog bolje preglednosti koda
ulaz_u_KO() {
    pthread_mutex_lock(&monitor);
}

izlaz_iz_KO() {
    pthread_mutex_unlock(&monitor);
}

void jesti(int n) {

        ulaz_u_KO(); // pthread_mutex_lock(&monitor);

            filozof[n] = 'o';

            while(vilica[n] == 0 || vilica[(n + 1) % 5] == 0) {
                pthread_cond_wait(&red[n], &monitor);
            }

            vilica[n] = vilica[(n + 1) % 5] = 0;
            filozof[n] = 'X';

            printf("%s (%d)\n", filozof, n + 1);

        izlaz_iz_KO(); // pthread_mutex_unlock(&monitor);

        sleep(2);

        ulaz_u_KO(); // pthread_mutex_lock(&monitor);

            filozof[n] = 'O';
            vilica[n] = vilica[(n + 1) % 5] = 1;

            if (n == 0) {
                pthread_cond_signal(&red[4]);
            } else {
                pthread_cond_signal(&red[n - 1]);
            }

            printf("%s (%d)\n", filozof, n + 1);

        izlaz_iz_KO(); // pthread_mutex_unlock(&monitor);
}
 
void misliti() {
    sleep(3);
}

void *filozofi(void *i) {
    while(1) {
        misliti();
        jesti((int) i);
    }
}


int main() {

    pthread_t dretve[5];

    pthread_cond_init(red, NULL);
    pthread_mutex_init(&monitor, NULL);

    for (int i = 0; i < 5; ++i) {
        pthread_create(&dretve[i], NULL, (void *) filozofi, (void *) i);
    }
    for (int i = 0; i < 5; ++i) {
        pthread_join(dretve[i], NULL);
    }

    return 0;
    exit(0);
}

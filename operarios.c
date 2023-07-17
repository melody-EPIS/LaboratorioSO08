#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10
#define M 20

sem_t semOP1, semOP2, semOP3;
int contadorOP1 = 0, contadorOP2 = 0, contadorOP3 = 0;

void *OP1(void *arg) {
    while (1) {
        sem_wait(&semOP1);
        contadorOP1++;
        printf("OP1 montó una rueda. Total: %d\n", contadorOP1);
        sem_post(&semOP2);
    }
}

void *OP2(void *arg) {
    while (1) {
        sem_wait(&semOP2);
        sem_wait(&semOP2);
        contadorOP2++;
        printf("OP2 montó un cuadro. Total: %d\n", contadorOP2);
        sem_post(&semOP3);
    }
}

void *OP3(void *arg) {
    while (1) {
        sem_wait(&semOP3);
        sem_wait(&semOP3);
        contadorOP3++;
        printf("OP3 montó un manillar. Total: %d\n", contadorOP3);
        if (contadorOP1 >= 2 && contadorOP2 >= 1) {
            contadorOP1 -= 2;
            contadorOP2 -= 1;
            printf("Montador ensambló una bicicleta. Ruedas: %d, Cuadros: %d\n", contadorOP1, contadorOP2);
        }
        sem_post(&semOP1);
    }
}

void *Montador(void *arg) {
    while (1) {
        sem_wait(&semOP1);
        sem_wait(&semOP1);
        sem_wait(&semOP2);
        sem_wait(&semOP3);
        printf("Montador ensambló una bicicleta.\n");
    }
}

int main() {
    pthread_t hiloOP1, hiloOP2, hiloOP3, hiloMontador;
    
    sem_init(&semOP1, 0, M);
    sem_init(&semOP2, 0, N);
    sem_init(&semOP3, 0, N);
    pthread_create(&hiloOP1, NULL, OP1, NULL);
    pthread_create(&hiloOP2, NULL, OP2, NULL);
    pthread_create(&hiloOP3, NULL, OP3, NULL);
    pthread_create(&hiloMontador, NULL, Montador, NULL);
    pthread_join(hiloOP1, NULL);
    pthread_join(hiloOP2, NULL);
    pthread_join(hiloOP3, NULL);
    pthread_join(hiloMontador, NULL);

    return 0;
}

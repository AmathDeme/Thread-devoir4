#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define N 10  // Limite de l'incrémentation et décrémentation

int valeur_partagee = 0;
sem_t sem_incremente, sem_decremente;  // Sémaphores pour contrôler l'accès

void* thread_incremente(void* arg) {
    int i;
    for (i = 1; i <= N; i++) {
        sem_wait(&sem_incremente);  // Attendre son tour
        valeur_partagee = i;
        printf("%d\n", valeur_partagee);
        sem_post(&sem_decremente);  // Donner le tour à l'autre thread
    }
    return NULL;
}

void* thread_decremente(void* arg) {
    int i;
    for (i = N; i >= -N; i--) {
        sem_wait(&sem_decremente);
        valeur_partagee = -i;
        printf("%d\n", valeur_partagee);
        sem_post(&sem_incremente);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Initialisation des sémaphores
    sem_init(&sem_incremente, 0, 1);  // Le premier thread commence
    sem_init(&sem_decremente, 0, 0);

    // Création des threads
    pthread_create(&t1, NULL, thread_incremente, NULL);
    pthread_create(&t2, NULL, thread_decremente, NULL);

    // Attendre que les threads terminent
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Détruire les sémaphores
    sem_destroy(&sem_incremente);
    sem_destroy(&sem_decremente);

    return 0;
}

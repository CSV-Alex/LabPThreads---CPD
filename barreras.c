#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int num_hilos;
int tipo_barrera; // 1mutex, 2semaforo, 3cond_var

// Variables para Barrera 1 (Mutex y Busy-wait)
int counter_1 = 0;
pthread_mutex_t barrier_mutex;

// Variables para Barrera 2 (Semáforos)
int counter_2 = 0;
sem_t count_sem;
sem_t barrier_sem;

// Variables para Barrera 3 (Variables de condición)
int counter_3 = 0;
pthread_mutex_t mutex_cond;
pthread_cond_t cond_var;

void barrera_mutex() {
    pthread_mutex_lock(&barrier_mutex);
    counter_1++;
    pthread_mutex_unlock(&barrier_mutex);
    while (counter_1 < num_hilos); // Busy-wait
}

void barrera_semaforo() {
    sem_wait(&count_sem);
    if (counter_2 == num_hilos - 1) {
        counter_2 = 0;
        sem_post(&count_sem);
        for (int j = 0; j < num_hilos - 1; j++) {
            sem_post(&barrier_sem);
        }
    } else {
        counter_2++;
        sem_post(&count_sem);
        sem_wait(&barrier_sem);
    }
}

void barrera_cond_var() {
    pthread_mutex_lock(&mutex_cond);
    counter_3++;
    if (counter_3 == num_hilos) {
        counter_3 = 0;
        pthread_cond_broadcast(&cond_var);
    } else {
        while (pthread_cond_wait(&cond_var, &mutex_cond) != 0);
    }
    pthread_mutex_unlock(&mutex_cond);
}

void* trabajo_hilo(void* arg) {
    long id = (long)arg;
    
    printf("Hilo %ld llego a la barrera...\n", id);
    
    if (tipo_barrera == 1) barrera_mutex();
    else if (tipo_barrera == 2) barrera_semaforo();
    else if (tipo_barrera == 3) barrera_cond_var();
    
    printf("Hilo %ld paso la barrera!\n", id);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: ./barreras <num_hilos> <tipo_barrera>\n");
        printf("Tipos: 1=Mutex, 2=Semaforo, 3=CondVar\n");
        return 1;
    }
    
    num_hilos = atoi(argv[1]);
    tipo_barrera = atoi(argv[2]);

    // Inicializar herramientas
    pthread_mutex_init(&barrier_mutex, NULL);
    sem_init(&count_sem, 0, 1);
    sem_init(&barrier_sem, 0, 0);
    pthread_mutex_init(&mutex_cond, NULL);
    pthread_cond_init(&cond_var, NULL);

    pthread_t* hilos = malloc(num_hilos * sizeof(pthread_t));

    for (long i = 0; i < num_hilos; i++) {
        pthread_create(&hilos[i], NULL, trabajo_hilo, (void*)i);
    }

    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Destruir herramientas
    pthread_mutex_destroy(&barrier_mutex);
    sem_destroy(&count_sem);
    sem_destroy(&barrier_sem);
    pthread_mutex_destroy(&mutex_cond);
    pthread_cond_destroy(&cond_var);
    
    free(hilos);
    printf("Todos los hilos pasaron la barrera.\n");
    return 0;
}
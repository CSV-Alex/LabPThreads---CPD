#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct nodo {
    int valor;
    struct nodo* siguiente;
} nodo;

pthread_mutex_t mutex_lista = PTHREAD_MUTEX_INITIALIZER;

nodo* head = NULL;
int num_hilos;
int inserts_por_hilo;

void list_insert(int v){
    nodo* nuevo = malloc(sizeof(nodo));
    nuevo->valor = v;

    pthread_mutex_lock(&mutex_lista);
    nuevo->siguiente = head;
    head = nuevo;
    pthread_mutex_unlock(&mutex_lista);
}

int list_buscar(int v){
    int encontrado = 0;
    pthread_mutex_lock(&mutex_lista);
    nodo* actual = head;
    while (actual != NULL){
        if (actual->valor == v){ encontrado = 1; break; }
        actual = actual->siguiente;
    }
    pthread_mutex_unlock(&mutex_lista);
    return encontrado;
}

int list_contar(){
    int count = 0;
    pthread_mutex_lock(&mutex_lista);
    nodo* actual = head;
    while (actual != NULL){
        count++;
        actual = actual->siguiente;
    }
    pthread_mutex_unlock(&mutex_lista);
    return count;
}

void* trabajo_hilo(void* arg){
    long id = (long)arg;
    int base = id * inserts_por_hilo;
    for (int i = 0; i < inserts_por_hilo; i++){
        list_insert(base + i);
        list_buscar(base + i);
    }
    return NULL;
}

int main(int argc, char* argv[]){
    num_hilos = 4;
    inserts_por_hilo = 1000;
    if (argc > 1) num_hilos = atoi(argv[1]);
    if (argc > 2) inserts_por_hilo = atoi(argv[2]);

    pthread_t* hilos = malloc(num_hilos * sizeof(pthread_t));

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < num_hilos; i++){
        pthread_create(&hilos[i], NULL, trabajo_hilo, (void*)i);
    }
    for (int i = 0; i < num_hilos; i++){
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    int esperado = num_hilos * inserts_por_hilo;
    int obtenido = list_contar();

    printf("hilos: %d, inserts_por_hilo: %d\n", num_hilos, inserts_por_hilo);
    printf("esperado: %d, obtenido: %d\n", esperado, obtenido);
    printf("tiempo: %.6f segundos\n", tiempo);

    nodo* actual = head;
    while (actual != NULL){
        nodo* sig = actual->siguiente;
        free(actual);
        actual = sig;
    }
    free(hilos);
    pthread_mutex_destroy(&mutex_lista);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct nodo {
    int valor;
    struct nodo* siguiente;
} nodo;

nodo* head = NULL;
int num_hilos;
int inserts_por_hilo;

void list_insert(int v){
    nodo* nuevo = malloc(sizeof(nodo));
    nuevo->valor = v;
    nuevo->siguiente = head;
    head = nuevo;
}

int list_buscar(int v){
    nodo* actual = head;
    while (actual != NULL){
        if (actual->valor == v) return 1;
        actual = actual->siguiente;
    }
    return 0;
}

int list_contar(){
    int count = 0;
    nodo* actual = head;
    while (actual != NULL){
        count++;
        actual = actual->siguiente;
    }
    return count;
}

void* trabajo_hilo(void* arg){
    long id = (long)arg;
    int base = id * inserts_por_hilo;
    for (int i = 0; i < inserts_por_hilo; i++){
        list_insert(base + i);
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
    if (obtenido != esperado) printf("RACE DETECTADA: se perdieron %d nodos\n", esperado - obtenido);
    printf("tiempo: %.6f segundos\n", tiempo);

    nodo* actual = head;
    while (actual != NULL){
        nodo* sig = actual->siguiente;
        free(actual);
        actual = sig;
    }
    free(hilos);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long num_pasos;
int num_hilos;
double ancho_paso;
double* sumas_locales;

void* calcular_pi_entre_hilos(void* arg){
    long id = (long)arg;
    double suma_local = 0.0;
    double x;

    long chunk = num_pasos / num_hilos;
    long inicio_paso = id * chunk;
    long fin_paso = (id == num_hilos - 1) ? num_pasos : (id + 1) * chunk;

    for (long i = inicio_paso; i < fin_paso; i++){
        x = (i + 0.5) * ancho_paso;
        suma_local += 4.0 / (1.0 + x * x);
    }

    sumas_locales[id] = suma_local;
    return NULL;
}

int main(int argc, char* argv[]){
    num_hilos = 4;
    num_pasos = 1000000000;
    if (argc > 1) num_hilos = atoi(argv[1]);
    if (argc > 2) num_pasos = atol(argv[2]);

    ancho_paso = 1.0 / num_pasos;
    sumas_locales = malloc(num_hilos * sizeof(double));
    pthread_t* hilos = malloc(num_hilos * sizeof(pthread_t));

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < num_hilos; i++){
        pthread_create(&hilos[i], NULL, calcular_pi_entre_hilos, (void*)i);
    }

    double suma_total = 0.0;
    for (int i = 0; i < num_hilos; i++){
        pthread_join(hilos[i], NULL);
        suma_total += sumas_locales[i];
    }

    double pi = suma_total * ancho_paso;

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("num_pasos: %ld, hilos: %d\n", num_pasos, num_hilos);
    printf("pi: %.10f\n", pi);
    printf("tiempo: %.6f segundos\n", tiempo);

    free(sumas_locales);
    free(hilos);
    return 0;
}
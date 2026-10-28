#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n, num_hilos;
double** A;
double* x;
double* y;

void* multiplicar_filas(void* arg){
    long id = (long)arg;

    int chunk = n / num_hilos;
    int inicio_fila = id * chunk;
    int fin_fila = (id == num_hilos - 1) ? n : (id + 1) * chunk;

    for (int i = inicio_fila; i < fin_fila; i++){
        double suma = 0.0;
        for (int j = 0; j < n; j++){
            suma += A[i][j] * x[j];
        }
        y[i] = suma;
    }
    return NULL;
}

int main(int argc, char* argv[]){
    n = 1000;
    num_hilos = 4;
    if (argc > 1) n = atoi(argv[1]);
    if (argc > 2) num_hilos = atoi(argv[2]);

    A = malloc(n * sizeof(double*));
    x = malloc(n * sizeof(double));
    y = malloc(n * sizeof(double));

    for (int i = 0; i < n; i++){
        A[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) A[i][j] = (i + j) % 100;
        x[i] = (i % 10) + 1;
    }

    pthread_t* hilos = malloc(num_hilos * sizeof(pthread_t));

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < num_hilos; i++){
        pthread_create(&hilos[i], NULL, multiplicar_filas, (void*)i);
    }
    for (int i = 0; i < num_hilos; i++){
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double checksum = 0.0;
    for (int i = 0; i < n; i++) checksum += y[i];

    printf("n: %d, hilos: %d\n", n, num_hilos);
    printf("y[0]: %.2f, y[n-1]: %.2f\n", y[0], y[n-1]);
    printf("checksum: %.4f\n", checksum);
    printf("tiempo: %.6f segundos\n", tiempo);

    for (int i = 0; i < n; i++) free(A[i]);
    free(A); free(x); free(y); free(hilos);
    return 0;
}
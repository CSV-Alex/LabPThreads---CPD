#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
    int n = 1000;
    if (argc > 1) n = atoi(argv[1]);

    double** A = malloc(n * sizeof(double*));
    double* x = malloc(n * sizeof(double));
    double* y = malloc(n * sizeof(double));

    for (int i = 0; i < n; i++){
        A[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) A[i][j] = (i + j) % 100;
        x[i] = (i % 10) + 1;
    }

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < n; i++){
        double suma = 0.0;
        for (int j = 0; j < n; j++){
            suma += A[i][j] * x[j];
        }
        y[i] = suma;
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double checksum = 0.0;
    for (int i = 0; i < n; i++) checksum += y[i];

    printf("n: %d\n", n);
    printf("y[0]: %.2f, y[n-1]: %.2f\n", y[0], y[n-1]);
    printf("checksum: %.4f\n", checksum);
    printf("tiempo: %.6f segundos\n", tiempo);

    for (int i = 0; i < n; i++) free(A[i]);
    free(A); free(x); free(y);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
    long num_pasos = 1000000000;
    if (argc > 1) num_pasos = atol(argv[1]);

    double suma = 0.0;
    double ancho_paso = 1.0 / num_pasos;
    double x, pi;
    struct timespec inicio, fin;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = 0; i < num_pasos; i++){
        x = (i + 0.5) * ancho_paso;
        suma += 4.0 / (1.0 + x * x);
    }

    pi = suma * ancho_paso;

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("num_pasos: %ld\n", num_pasos);
    printf("pi: %.10f\n", pi);
    printf("tiempo: %.6f segundos\n", tiempo);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // Include for rand_r and RAND_MAX
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    unsigned long long local_count = 0, global_count = 0;
    long long i;
    long long samples;
    double x, y;

     // Tiempo de inicio

    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    samples = 300000; // Valor por defecto
    if (argc > 1) // Para su uso en línea de comandos
        samples = atoll(argv[1]);
    
    double pepe = MPI_Wtime();	
    unsigned int seed = rank;

    for (i = 0; i < samples/size; ++i) {
        x = ((double)rand_r(&seed)) / ((double)RAND_MAX); // 0 ≤ x ≤ 1
        y = ((double)rand_r(&seed)) / ((double)RAND_MAX);
        if (x * x + y * y <= 1.0) {
            local_count+=1;
        }
    }

    // Sumar el conteo local de cada proceso
    MPI_Reduce(&local_count, &global_count, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    // Proceso 0 imprime el resultado
    if (rank == 0) {
	printf("Numero de hilos: %d \n", size);
        printf("Valor estimado de pi: %.7f\n", 4.0 * global_count / samples);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double juan = MPI_Wtime(); // Tiempo de finalización
    double resta = juan - pepe; // Calcular diferencia de tiempo

    if (rank == 0) {
    printf("Tiempo total tardado: %f segundos\n", resta);
}
    MPI_Finalize();
    return 0;
}
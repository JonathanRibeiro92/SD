#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);

    int vector[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    int local_sum = 0;
    int part = 12/world_size;
    int init = part * world_rank;
    int i;
    for (i = init; i < (init+part); i++) {
        local_sum += vector[i];
    }

    
    printf("Soma local do processo(%d) : %d\n",
            world_rank, local_sum);

    
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0,
                MPI_COMM_WORLD);

    
    if (world_rank == 0) {
        printf("Soma Total = %d\n", global_sum);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
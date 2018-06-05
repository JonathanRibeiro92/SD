#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_VET 100



int main(int argc, char *argv[])
{
	
	int size,rank;
	
    int vetor_rcv[TAM_VET/2];

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

	//printf("Rank %d de um total de %d\n",rank,size);
	
	if (rank==0)
	{
		//criando e preenchendo o vetor
		int vetor_snd[TAM_VET];

		int part = TAM_VET/size;

		int pos_vetor;
		printf("Ola sou o processo 0\n");

		for (int i = 0; i < TAM_VET; i++)
		{
			int randonNumber;
			randonNumber = rand()%100;
			vetor_snd[i] = randonNumber;
		}

		for (int rank_i = 0; rank_i < size; rank_i++)
        {
            pos_vetor = rank_i * part;
            printf("SEND %d\t%d\t%d\n", rank_i, part, pos_vetor);
            MPI_Send(&vetor_snd[pos_vetor], part, MPI_INT, rank_i, 123, MPI_COMM_WORLD);
		} 


	}
    int part = TAM_VET / size;
    
    MPI_Recv(vetor_rcv, part, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int min = vetor_rcv[0] * 0.9;
    for (int i = 1; i < part; i++)
    {
        int multiply = vetor_rcv[i] * 0.9;
        if ( multiply < min ) 
        {
           min = multiply;
        }

    }

    printf("Rank %d\t%d\n", rank, min);

    MPI_Send(&min, 1, MPI_INT, 0, 124, MPI_COMM_WORLD);
    
    

    MPI_Finalize();
    return(0);
}	
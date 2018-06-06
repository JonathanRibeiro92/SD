/*                                                                          */
/****************************************************************************/
/* Programacao Paralela e Distribuida                                       */
/*                                                                          */
/* Prof: Sergio Nery Simoes                                                 */
/*                                                                          */
/* Exercicio: Modelo Master/Slave em MPI                                    */
/* Descricao: Implementacao do modelo master/slave em MPI                   */
/*                                                                          */
/* Compilar: mpicc mpi_master_slave.c -o mpi_master_slave -Wall             */
/* Executar: mpirun -np 4 mpi_master_slave 10000                            */
/*                                                                          */
/****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define TAG_WORK 1
#define TAG_END  2

int master(int rank, int size, int nworks, int *result)
{
	int ind, rank_i, work=0, total_works=nworks, min_sendrecv;
	MPI_Status status;
	printf("1-master sends 1 work to all slaves\n");
	// master sends 1 work to all slaves
    min_sendrecv = (size < total_works) ? size : total_works;
	for (rank_i=1; rank_i<min_sendrecv; rank_i++)
	{
		MPI_Send(&work, 1, MPI_INT, rank_i, TAG_WORK, MPI_COMM_WORLD);
		work++;
	}
	printf("2-master receive results from slaves and sends new works until works ends\n");
	// master receive results from slaves and sends new works until works ends
	while (work < total_works)
	{
		MPI_Recv(&ind, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		result[ind] = ind;
		MPI_Send(&work, 1, MPI_INT, status.MPI_SOURCE, TAG_WORK, MPI_COMM_WORLD);
		work++;
	}
	printf("3-master receives last results from all slaves\n");
	// master receives last results from all slaves
	for (rank_i=1; rank_i<min_sendrecv; rank_i++)
	{
		MPI_Recv(&ind, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		result[ind] = ind;
	}
	printf("4-master sends termination TAG\n");
	// master sends termination TAG
	for (rank_i=1; rank_i<size; rank_i++)
	{
		MPI_Send(0, 0, MPI_INT, rank_i, TAG_END, MPI_COMM_WORLD);
	}
	return 0;
}

int slave(int rank)
{
	int work, result;
	MPI_Status status;
	for (;;)
	{
		MPI_Recv(&work, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if (status.MPI_TAG==TAG_END)
			break;
		/* slave computes its works */
		result = work;
		/* slave sends results */
		MPI_Send(&result, 1, MPI_INT, 0, TAG_WORK, MPI_COMM_WORLD);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int i, rank, size, total_works=100, *result;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (argc>1)
		total_works = atoi(argv[1]);
	if (rank == 0)
	{
		result = (int*) calloc(total_works, sizeof(int));
		master(rank, size, total_works, result);
		for (i=0; i<total_works; i++)
			printf("res[%d]=%3d\t", i, result[i]);
		printf("\n");
	}
	else
	{
		slave(rank);
	}	
	MPI_Finalize();
	return 0;
}


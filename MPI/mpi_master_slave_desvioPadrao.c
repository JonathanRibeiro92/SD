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
#include <math.h>
#include <time.h>

#define TAG_WORK 1
#define TAG_END  2

/*Vetor global que armazena a soma e a soma do quadrado dos elementos de cada processo
onde result[0] = soma e result[1] = soma dos quadrados*/
int result[2];

float *init_vector(int v_size)
{
	float * vetor = calloc(v_size, sizeof(float));
	
	int i;

	srand(time(NULL));
	for (i=0; i<v_size; i++)
	{	
		vetor[i] = rand()%100;
	}
	return vetor;
}

float media(float *vetor, int tam){
	
	float m = 0.0;

	for(int i = 0; i < tam; i++)
	{
		m += vetor[i];
	}
	m = m/tam;
	
	return m;
}

float quadradoDist(float elem, float m){

	float quadrado = 0.0;
	quadrado = pow((elem - m),2);
	return quadrado;
}

float * fnc_soma_quadrado(float *vetor, int tam_vet)
{
	float *vet_result = calloc (2, sizeof(float));
	float soma = 0.0, quadrado = 0.0;
	
	for(int i = 0; i < tam_vet ; i++)
	{
		soma += vetor[i];
		quadrado += vetor[i] * vetor[i];
	}
	
	
	vet_result[0] = soma;
	vet_result[1] = quadrado;
	
	return vet_result; 
}

float Calcula_desvio_padrao(int n)
{
	float x_linha, desv_pad;
		
	x_linha = result[0] / n; /*Média = soma de todos os elementos dividido pelo numero de elementos*/
	return desv_pad = (result[1] - 2 * (result[0] * x_linha) + (n * (x_linha * x_linha))) /n;
}

int master(int rank, int size, int nworks, float *desvio_padrao)
{
	int rank_i, work=0, total_works=nworks, min_sendrecv;
	int tam_vet = 100;
	int pos = 0;
	int tot_pos = 10;

	float vetor_rcv[2];

	MPI_Status status;

	float *vetor = init_vector(size);


	printf("1-master sends 1 work to all slaves\n");
	// master sends 1 work to all slaves
    min_sendrecv = (size < total_works) ? size : total_works;
	for (rank_i=1; rank_i<min_sendrecv; rank_i++)
	{
		MPI_Send(&vetor[pos], 10, MPI_FLOAT, rank_i, TAG_WORK, MPI_COMM_WORLD);
		work++;
		pos += tot_pos;
	}
	printf("2-master receive results from slaves and sends new works until works ends\n");
	// master receive results from slaves and sends new works until works ends
	while (pos < total_works)
	{
		MPI_Recv(&vetor_rcv, 2, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		result[0] = result[0] + vetor_rcv[0];
		result[1] = result[1] + vetor_rcv[1];
		
		MPI_Send(&vetor_rcv[pos], 10, MPI_FLOAT, status.MPI_SOURCE, TAG_WORK, MPI_COMM_WORLD);
		work++;
		pos = pos + tot_pos;
	}
	printf("3-master receives last results from all slaves\n");
	// master receives last results from all slaves
	for (rank_i=1; rank_i<min_sendrecv; rank_i++)
	{
		MPI_Recv(&vetor_rcv, 2, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		result[0] = result[0] + vetor_rcv[0];
		result[1] = result[1] + vetor_rcv[1];
	}
	printf("4-master sends termination TAG\n");
	// master sends termination TAG
	for (rank_i=1; rank_i<size; rank_i++)
	{
		MPI_Send(0, 0, MPI_FLOAT, rank_i, TAG_END, MPI_COMM_WORLD);
	}

	printf("total das somas = %d total da soma dos quadrados = %d\n",result[0], result[1]);
	
	/*Calcula o desvio padrao*/
	*desvio_padrao = Calcula_desvio_padrao(tam_vet);
	return 0;
}

int slave(int rank)
{
	int tam_vetor_aux = 10;
	float vetor_rcv [tam_vetor_aux];
	float *result_local;

	result_local = calloc(2,sizeof(float));
	
	MPI_Status status;
	
	for (;;)
	{
		MPI_Recv(&vetor_rcv, 10, MPI_FLOAT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if (status.MPI_TAG==TAG_END)
			break;
		result_local = fnc_soma_quadrado(vetor_rcv,tam_vetor_aux);
		MPI_Send(result_local, 2, MPI_FLOAT, 0, TAG_WORK, MPI_COMM_WORLD);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int i, rank, size, total_works=100;
	float *desvio_padrao;

	if (argc<1)
	{
		printf("Número de parametros insuficiente...\n");
   		exit(-1);
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	
	if (rank == 0)
	{
		//*desvio_padrao = 0.0;
		
		master(rank, size, total_works, desvio_padrao);
		
		printf("O valor do desvio padrão é: %.2f\n", *desvio_padrao);
		printf("\n");
	}
	else
	{
		slave(rank);
	}	
	MPI_Finalize();
	return 0;
}
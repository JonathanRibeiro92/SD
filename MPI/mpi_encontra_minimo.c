#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define TAM_VET 100000
#define EPSILON 0.00009


int main(int argc, char *argv[])
{
	
	int size,rank;
	srand(time(NULL));
    float vetor_rcv[TAM_VET];
    float min_local = 10000;
    float global_min;
    int interacoes_total;

    MPI_Init(&argc,&argv);


    for(int i = 0;i<TAM_VET;i++)
    {
        float r = (rand() % 100)+1;
        vetor_rcv[i] = r;
    }

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);


    int part = TAM_VET/size;
	//printf("Rank %d de um total de %d\n",rank,size);
	
	// if (rank==0)
	// {
	// 	//criando e preenchendo o vetor
	// 	//Passo 1
    //     int vetor_snd[TAM_VET];

	// 	int part = TAM_VET/size;

	// 	int pos_vetor;
	// 	printf("Ola sou o processo 0\n");

	// 	for (int i = 0; i < TAM_VET; i++)
	// 	{
	// 		int randonNumber;
	// 		randonNumber = rand()%100;
	// 		vetor_snd[i] = randonNumber;
	// 	}


    //     //Passo 2 - esse processo distribuirá o vetor em N processos (P = k.N, onde k=1,2,3,...) 
    //     //-- incluindo a si próprio.

	// 	for (int rank_i = 0; rank_i < size; rank_i++)
    //     {
    //         pos_vetor = rank_i * part;
    //         printf("SEND %d\t%d\t%d\n", rank_i, part, pos_vetor);
    //         MPI_Send(&vetor_snd[pos_vetor], part, MPI_INT, rank_i, 123, MPI_COMM_WORLD);
	// 	} 


	// }
    int part = TAM_VET / size;
    int init = part * rank;
    

    //Passo 3 - Em cada iteração, os processos deverão multiplicar os elementos
    // de seu vetor local por 0.9.

    while(1)
    {
        for (i = init; i < (init+part); i++) {
            interacoes += 1;
            vector[i] *= 0.9;
            if(vector[i] <= min_local)
            {
                min_local = vector[i];
                if(min_local <= EPSILON)
                {
                    break;
                }
            }
            
        }
       // Passo 4 - Em seguida, cada processo deverá encontrar individualmente
       // o MENOR VALOR LOCAL (local_min) e compartilhar este valor com os demais processos para que,
       // ao final da operação, TODOS saibam qual o menor valor GLOBAL (global_min).
        MPI_Allreduce(&min_local, &global_min, 1, MPI_FLOAT, MPI_MIN, MPI_COMM_WORLD); 
        //Passo 5 - CONDIÇÃO DE PARADA (global_min <= epsilon).
        if(global_min <= EPSILON)
        {
            break;
        }    
    }

    // MPI_Recv(vetor_rcv, part, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // int min = vetor_rcv[0] * 0.9;
    // for (int i = 1; i < part; i++)
    // {
    //     int multiply = vetor_rcv[i] * 0.9;
    //     if ( multiply < min ) 
    //     {
    //        min = multiply;
    //     }

    // }

    printf("Rank #%d\tNumero de interacoes: %d\tMinimo encontrado: %f\n", rank, interacoes, min_local);

    //MPI_Send(&min, 1, MPI_INT, 0, 124, MPI_COMM_WORLD);
    

    MPI_Reduce(&interacoes, &interacoes_total, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);

    //Passo 6 -  Um dos processos (rank=0) imprimirá:
    //número de iterações e o menor valor encontrado;
    if (world_rank == 0) {
        printf("Minimo global = %f, Interações total = %d \n", global_min, interacoes_total);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    return(0);
}	
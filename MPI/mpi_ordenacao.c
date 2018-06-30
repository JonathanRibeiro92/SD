#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define P 1000



void quickSort(int *vetor, int inicio, int fim)
{
    int i, j, meio, aux;

    i = inicio;
    j = fim;
    meio = vetor[(inicio + fim) / 2];

    do
    {
        while(vetor[i] < meio)
            i++;
        while(vetor[j] > meio)
            j--;
        if(i <= j)
        {
            aux = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = aux;
            i++;
            j--;
        }
    }while(i <= j);

    if(inicio < j)
        quickSort(vetor, inicio, j);
    if(i < fim)
        quickSort(vetor, i, fim);
}




int main(int argc, char *argv[])
{
	
	int size,rank;
    int vetor_rcv[TAM_VET];
    
    
    MPI_Init(&argc,&argv);


    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);


    int part = P/size;
	//printf("Rank %d de um total de %d\n",rank,size);
	int pos_vetor;
	if (rank==0)
	{
		//criando e preenchendo o vetor
		//Passo 1
        int vetor_snd[P];

		int part = P/size;

		pos_vetor = 0;
		printf("Ola sou o processo 0\n");

		for (int i = 0; i < P; i++)
		{
			int randonNumber;
			randonNumber = rand()%100;
			vetor_snd[i] = randonNumber;
		}


        //Passo 2 - esse processo distribuirá o vetor em N processos (P = k.N, onde k=1,2,3,...) 
        //-- incluindo a si próprio.

		for (int rank_i = 0; rank_i < size; rank_i++)
        {
            pos_vetor = rank_i * part;
            printf("SEND %d\t%d\t%d\n", rank_i, part, pos_vetor);
            MPI_Send(&vetor_snd[pos_vetor], part, MPI_INT, rank_i, 123, MPI_COMM_WORLD);
		} 


	}
    
    int init = part * rank;
    

    //Passo 3 - Em cada iteração, os processos deverão multiplicar os elementos
    // de seu vetor local por 0.9.

    while(1)
    {
        for (int i = init; i < (init+part); i++) {
            interacoes += 1;
            vetor_rcv[i] *= 0.9;
            if(vetor_rcv[i] <= min_local)
            {
                min_local = vetor_rcv[i];
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

    printf("Rank #%d Numero de interacoes: %d Minimo encontrado: %f\n", rank, interacoes, min_local);

    //MPI_Send(&min, 1, MPI_INT, 0, 124, MPI_COMM_WORLD);
    

    MPI_Reduce(&interacoes, &interacoes_total, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);

    //Passo 6 -  Um dos processos (rank=0) imprimirá:
    //número de iterações e o menor valor encontrado;
    if (rank == 0) {
        printf("Minimo global = %f, Interações total = %d \n", global_min, interacoes_total);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    return(0);
}	
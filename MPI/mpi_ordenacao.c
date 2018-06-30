#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define P 100



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
    int vetor_snd[P];
    
    
    MPI_Init(&argc,&argv);


    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int part = P/size;
    int vetor_rcv[part];


	//printf("Rank %d de um total de %d\n",rank,size);
	int pos_vetor;
	if (rank==0)
	{
		//criando e preenchendo o vetor
		//Passo 1


		for (int i = 0; i < P; i++)
		{
			int randonNumber;
			randonNumber = rand()%100;
			vetor_snd[i] = randonNumber;
		}
        

        //Passo 2 - esse processo distribuirá o vetor em N processos (P = k.N, onde k=1,2,3,...) 
        //-- incluindo a si próprio.

        
        quickSort(vetor_snd,0,part);
        
		for (int rank_i = 1; rank_i < size; rank_i++)
        {
            pos_vetor = rank_i * part;
            //printf("SEND %d\t%d\t%d\n", rank_i, part, pos_vetor);
            MPI_Send(&vetor_snd[pos_vetor], part, MPI_INT, rank_i, 123, MPI_COMM_WORLD);
		} 


        for (int rank_i = 1; rank_i < size; rank_i++)
        {
            pos_vetor = rank_i * part;
            //printf("SEND %d\t%d\t%d\n", rank_i, part, pos_vetor);
            MPI_Recv(&vetor_snd[pos_vetor], part, MPI_INT, rank_i, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

        quickSort(vetor_snd,0,part);

        printf("vetor ordenado: \n");
        for (int i=0; i<P; i++)
        {
            printf("Rank %d - vetor[%d] = %d\n", rank, i, vetor_snd[i]);
        }



	}else{
        MPI_Recv(&vetor_rcv, part, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        quickSort(vetor_rcv,0,part-1);
        
        printf("vetor local rank %d ordenado \n", rank);

        MPI_Send(&vetor_rcv, part, MPI_INT, 0, 123, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return(0);
}	
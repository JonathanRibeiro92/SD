#include <mpi.h>
#include <stdio.h>
#include <string.h>

/* This code passes a part of the vector for each thread to print its values */

#define TAM_VET 12

int main(int argc, char *argv[])
{
    int size,rank;

    int vetor_snd[]={10,20,30,40,50,60,70,80,90,100,110,120};
    int vetor_rcv[TAM_VET/2];

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    printf("Rank %d de um total de %d\n",rank,size);

    if(rank==0)
    {
        printf("Ola sou o processo 0\n");

        int part = TAM_VET /size;

        int pos_vetor;

        for(int rank_i=1; rank_i<size;rank_i++)
        {
            pos_vetor = rank_i * part;
            printf("SEND %d\t%d\t%d\n",rank_i,part,pos_vetor);
            MPI_Send(&vetor_snd[pos_vetor],part,MPI_INT,rank_i,123, MPI_COMM_WORLD);
        }
    }else{
        int part = TAM_VET /size;
        MPI_Recv(vetor_rcv,part,MPI_INT,0,123,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        for(int i = 0; i<part;i++)
        {
            printf("rank %d\t%d\n",rank,vetor_rcv[i]);
        }

    }
    MPI_Finalize();
    return 0;
}
 
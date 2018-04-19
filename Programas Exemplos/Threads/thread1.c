#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* Para compilar:
	gcc thread1.c -o thread1 -lpthread */

void* funcao_thread(void* arg)
{
	printf("\tFILHA: sou a nova thread e aguardarei 10s.\n");
	sleep(10);
	printf("\tFILHA: terminarei agora.\n");
	return NULL;
}
int main()
{
	pthread_t thread;
	printf("PAI: criando uma nova thread...\n");
	pthread_create(&thread, NULL, funcao_thread, NULL);

	printf("PAI: aguardando a conclusao da thread...\n");
	pthread_join(thread, NULL);
	printf("PAI: a thread terminou. Vou terminar tambem.\n");
	return 0;
}


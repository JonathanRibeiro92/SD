#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void funcao1(char* str)
{
	int i;
	for (i=0; i<10000000; i++)
		printf("\t11111111\n");
	printf("\tTerminando a thread 1.\n");
}
void funcao2(char* str)
{
	int i;
	for (i=0; i<10000000; i++)
		printf("\t\t22222222\n");
	printf("\tTerminando a thread 2.\n");
}
int main()
{
	pthread_t thread_a, thread_b;
	printf("Pai: Criando duas threads...\n");
	pthread_create(&thread_a, NULL, (void*)funcao1, NULL);
	pthread_create(&thread_b, NULL, (void*)funcao2, NULL);

	printf("Pai: Aguardando as duas threads...\n");
	pthread_join(thread_a, NULL);
	pthread_join(thread_b, NULL);
	printf("Ambas as thread terminaram...\n");
	return 0;
}


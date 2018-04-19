#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* funcao_thread(char* str)
{
	printf("\tRecebi o argumento (%s) do processo pesado\n", str);
	sleep (2);
	printf("\tTerminando a thread que recebeu o argumento (%s).\n", str);
	return 0;
}
int main()
{
	void* retval;
	pthread_t thread_a, thread_b;
	printf("Pai: Criando duas threads...\n");
	pthread_create(&thread_a, NULL, funcao_thread, "AAA");
	pthread_create(&thread_b, NULL, funcao_thread, "BBB");
	printf("Pai: Aguardando as duas threads...\n");
	pthread_join(thread_a, &retval);
	pthread_join(thread_b, &retval);
	printf("Pai: Aguardando as duas threads...\n");
	return 0;
}


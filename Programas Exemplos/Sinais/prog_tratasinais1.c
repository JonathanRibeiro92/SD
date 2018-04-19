#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void tratasinal(int sinal)
{
	printf("Sinal recebido: %d.\n", sinal);
}

int main()
{
	signal(1, tratasinal);
	signal(2, tratasinal);
	signal(3, tratasinal);
	signal(4, tratasinal);
	signal(5, tratasinal);
	signal(6, tratasinal);
	signal(7, tratasinal);
	signal(8, tratasinal);
	signal(9, tratasinal);
	signal(10, tratasinal);
	signal(11, tratasinal);
	signal(12, tratasinal);
	signal(13, tratasinal);
	signal(14, tratasinal);
	//signal(15, tratasinal);
	printf("Vou entrar em loop infinito.\n");
	for (;;)
		;
	return 0; /* <-- esta linha nao sera executada */
}


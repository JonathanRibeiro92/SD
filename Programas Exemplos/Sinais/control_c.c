#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void meualarme(int sinal)
{
	printf("Voce pressionou control+c (%d). "
		"Espere 5 segundos para encerrar o programa.\n", sinal);
	sleep(5);
	exit(0);
}

int main()
{
	signal(SIGINT, meualarme);
	printf("Vou entrar em loop infinito.\n");
	for (;;) ;
}

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void meualarme(int sinal)
{
	printf("Sinal recebido %d. Terminando o programa.\n", sinal);
	exit(0);
}

int main()
{
	signal(14, meualarme);
	printf("Enviarei um alarme daqui a 5 segundos.\n");
	alarm(5);
	printf("Vou entrar em loop infinito.\n");
	for (;;) ;
}

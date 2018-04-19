#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void nomesinal(int sinal, char* str)
{
	switch(sinal)
	{
		case 1:  strcpy(str, "SIGHUP");  break;
		case 2:  strcpy(str, "SIGINT");  break;
		case 3:  strcpy(str, "SIGQUIT"); break;
		case 4:  strcpy(str, "SIGILL");  break;
		case 5:  strcpy(str, "SIGTRAP"); break;
		case 6:  strcpy(str, "SIGABRT"); break;
		case 7:  strcpy(str, "SIGBUS");  break;
		case 8:  strcpy(str, "SIGFPE");  break;
		case 9:  strcpy(str, "SIGKILL"); break;
		case 10: strcpy(str, "SIGUSR1");  break;
		case 11: strcpy(str, "SIGSEGV"); break;
		case 12: strcpy(str, "SIGUSR2"); break;
		case 13: strcpy(str, "SIGPIPE"); break;
		case 14: strcpy(str, "SIGALRM"); break;
		case 15: strcpy(str, "SIGTERM"); break;
	}
}

void tratasinal(int sinal)
{
	char nome[20];
	nomesinal(sinal, nome);
	printf("Sinal recebido: %d (%s)\n", sinal, nome);
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
	signal(15, tratasinal);
	printf("Vou entrar em loop infinito.\n");
	for (;;)
		sleep (1);
	return 0; /* <-- esta linha nao sera executada */
}


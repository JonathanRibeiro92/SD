#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* variavel global - compartilhada entre as threads

Descrição:
Visto que as threads compartilham a área de dados, área de código e contexto de software,
é possível ocorrer problemas durante o acesso compartilhado.
Este programa demonstra o problema do compartilhamento de recursos que ocorre devido às 
condições de corrida devido ao acesso simultaneo em uma variavel global compartilhada.

Uma variável global é utilizada simultaneamente por duas threads e o resultado nem sempre
sai conforme esperado.

Atividade
1) Observe o programa. Se não houver nenhum problema qual deve ser o valor final da 
variavel 'conta' a ser impresso?
2) Digite o programa e compile-o. Em seguida execute o programa diversas vezes 
(de 10 a 20 vezes seguidamente) e observe o comportamento do programa.
3) Porque nem sempre os valores são impressos conforme o esperado?
4) Proponha uma solução, mas não precisa implementar, para resolver o problema.
*/

int conta=0;

void *funcao1()
{
	int i;
	for (i=0; i<100000000; i++)
		conta = conta + 3;
	printf("Encerrei a thread 1\n");
	return 0;
}
void *funcao2()
{
	int i;
	for (i=0; i<100000000; i++)
		conta = conta + 7;
	printf("Encerrei a thread 2\n");
	return 0;
}
int main()
{
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, funcao1, NULL);
	pthread_create(&thread2, NULL, funcao2, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	printf("O valor da conta eh: %d\n", conta);
	return 0;
}


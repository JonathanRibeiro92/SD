#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

/*=====================================================================
Descricao: Este programa calcula a integral atraves de duas threads
Autor: Sergio Nery Simoes

Para compilar:
    gcc thread_integral.c -o thread_integral -lpthread -lm -Wall

Para executar:
    time ./thread_integral 0.0 1.0 200000000

========================================================================*/

double integral = 0.0;

typedef struct
{
    double a;
    double b;
    int N;
} Targumentos;

double AreaTrapezio(double dx, double h1, double h2)
{
    return (dx*(h1+h2)/2);
}

double f(double x)
{
    return (4*sqrt(1-x*x));
}

void ThreadCalculaArea(Targumentos *argumentos)
{
    int i;
    double area, dx, x1, x2, f1, f2;
    Targumentos arg;
    arg = *argumentos;
    double a=arg.a; double b=arg.b; int N=arg.N;
    printf("a=%10.1lf\tb=%10.1lf\tN=%d\n", a, b, N);
    dx   = (b-a)/N;
    area = 0.0;
    for (i=0; i<N; i++)
    {
        x1 = a + dx * i;
        x2 = a + dx * (i+1);
        f1 = f(x1);
        f2 = f(x2);
        area += AreaTrapezio(dx, f1, f2);
    }
    integral += area;
}

int main(int argc, char **argv)
{
    double a, b;
    int N;
    pthread_t thread_1, thread_2;
    Targumentos arg1, arg2;
    if (argc<4)
    {
        printf("Numero de argumentos insuficiente...\n");
        exit(-1);
    }
    a = (double) atof(argv[1]);
    b = (double) atof(argv[2]);
    N = atoi(argv[3]);

    /* arg1 */
    arg1.a = a,
    arg1.b = (a+b)/2,
    arg1.N = N/2;
    /* arg2 */
    arg2.a = (a+b)/2,
    arg2.b = b,
    arg2.N = N/2;

    pthread_create(&thread_1, NULL, (void*)ThreadCalculaArea, &arg1);
    pthread_create(&thread_2, NULL, (void*)ThreadCalculaArea, &arg2);

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    printf("Area= %.15lf\n", integral);
    return 0;
}


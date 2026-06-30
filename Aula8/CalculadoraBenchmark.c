#include <stdio.h>
#include <time.h>

long long soma(long long a, long long b)
{
    return a + b;
}

long long subtracao(long long a, long long b)
{
    return a - b;
}

long long multiplicacao(long long a, long long b)
{
    return a * b;
}

long long divisao(long long a, long long b)
{
    return a / b;
}

unsigned long long fatorial(int n)
{
    unsigned long long resultado = 1;

    for(int i=2;i<=n;i++)
        resultado*=i;

    return resultado;
}

double tempoMicro(struct timespec inicio,
                  struct timespec fim)
{

    double segundos =
        fim.tv_sec - inicio.tv_sec;

    double nanos =
        fim.tv_nsec - inicio.tv_nsec;

    return segundos*1000000.0 +
           nanos/1000.0;
}

int main()
{

    int op;

    long long A,B;

    struct timespec inicio,fim;

    printf("====== Benchmark ======\n\n");

    printf("1 Soma\n");
    printf("2 Subtracao\n");
    printf("3 Multiplicacao\n");
    printf("4 Divisao\n");
    printf("5 Fatorial\n");

    printf("\nEscolha: ");
    scanf("%d",&op);

    if(op==5)
    {

        int n;

        printf("Digite n: ");
        scanf("%d",&n);

        clock_gettime(CLOCK_MONOTONIC,
                      &inicio);

        unsigned long long r =
            fatorial(n);

        clock_gettime(CLOCK_MONOTONIC,
                      &fim);

        printf("\nResultado: %llu\n",r);

        printf("Tempo: %.3lf us\n",
               tempoMicro(inicio,fim));

        return 0;

    }

    printf("Digite A: ");
    scanf("%lld",&A);

    printf("Digite B: ");
    scanf("%lld",&B);

    if(op==4 && B==0)
    {
        printf("\nErro: divisao por zero.\n");
        return 0;
    }

    long long resultado=0;

    clock_gettime(CLOCK_MONOTONIC,
                  &inicio);

    switch(op)
    {

        case 1:
            resultado=soma(A,B);
            break;

        case 2:
            resultado=subtracao(A,B);
            break;

        case 3:
            resultado=multiplicacao(A,B);
            break;

        case 4:
            resultado=divisao(A,B);
            break;

        default:
            printf("Operacao invalida.\n");
            return 0;
    }

    clock_gettime(CLOCK_MONOTONIC,
                  &fim);

    printf("\nResultado: %lld\n",
           resultado);

    printf("Tempo: %.3lf us\n",
           tempoMicro(inicio,fim));

    return 0;
}

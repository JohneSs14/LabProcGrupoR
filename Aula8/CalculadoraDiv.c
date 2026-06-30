#include <stdio.h>

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
    if(n < 0)
        return 0;

    unsigned long long resultado = 1;

    for(int i = 2; i <= n; i++)
        resultado *= i;

    return resultado;
}

int main()
{
    int op;

    long long A;
    long long B;

    printf("==============================\n");
    printf(" Calculadora - Raspberry Pi\n");
    printf("==============================\n\n");

    printf("1 - Soma\n");
    printf("2 - Subtracao\n");
    printf("3 - Multiplicacao\n");
    printf("4 - Fatorial\n");
    printf("5 - Divisao\n\n");

    printf("Escolha: ");
    scanf("%d",&op);

    // Fatorial
    if(op == 4)
    {
        int n;

        printf("\nDigite n: ");
        scanf("%d",&n);

        if(n < 0)
        {
            printf("\nNao existe fatorial de numero negativo.\n");
            return 0;
        }

        printf("\nResultado = %llu\n",
               fatorial(n));

        return 0;
    }

    // Outras operações

    printf("\nDigite A: ");
    scanf("%lld",&A);

    printf("Digite B: ");
    scanf("%lld",&B);

    if(op == 5 && B == 0)
    {
        printf("\nERRO: divisao por zero.\n");
        printf("A operacao foi cancelada para evitar uma excecao de execucao.\n");
        return 0;
    }

    long long resultado;

    switch(op)
    {
        case 1:
            resultado = soma(A,B);
            break;

        case 2:
            resultado = subtracao(A,B);
            break;

        case 3:
            resultado = multiplicacao(A,B);
            break;

        case 5:
            resultado = divisao(A,B);
            break;

        default:
            printf("\nOperacao invalida.\n");
            return 0;
    }

    printf("\nResultado = %lld\n",resultado);

    return 0;
}

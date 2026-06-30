#include <stdio.h>
#include <stdbool.h>

#define MIN_4BITS -8
#define MAX_4BITS 7

// Verifica se um número cabe em 4 bits signed
bool overflow4bits(int valor)
{
    return (valor < MIN_4BITS || valor > MAX_4BITS);
}

// Imprime representação binária (4 bits)
void imprimeBinario4Bits(int valor)
{
    unsigned int numero = valor & 0x0F;

    printf("Binario: ");

    for(int i = 3; i >= 0; i--)
    {
        printf("%d", (numero >> i) & 1);
    }

    printf("\n");
}

int soma(int a, int b)
{
    return a + b;
}

int subtracao(int a, int b)
{
    return a - b;
}

int multiplicacao(int a, int b)
{
    return a * b;
}

int fatorial(int n)
{
    if(n < 0)
        return -9999;

    int resultado = 1;

    for(int i = 2; i <= n; i++)
        resultado *= i;

    return resultado;
}

void mostraResultado(int resultado)
{
    if(overflow4bits(resultado))
    {
        printf("\n***************\n");
        printf("OVERFLOW!\n");
        printf("Resultado nao cabe em 4 bits.\n");
        printf("Resultado real = %d\n", resultado);
        printf("***************\n");
        return;
    }

    printf("\nResultado decimal: %d\n", resultado);

    imprimeBinario4Bits(resultado);
}

int main()
{

    int A;
    int B;
    int op;
    int resultado;

    printf("==============================\n");
    printf(" ALU 4 Bits - Raspberry Pi\n");
    printf("==============================\n\n");

    printf("Operacoes:\n");
    printf("1 - Soma\n");
    printf("2 - Subtracao\n");
    printf("3 - Multiplicacao\n");
    printf("4 - Fatorial\n\n");

    printf("Escolha: ");
    scanf("%d",&op);

    if(op == 4)
    {
        printf("\nDigite um numero (-8 ate 7): ");
        scanf("%d",&A);

        if(overflow4bits(A))
        {
            printf("Numero invalido!\n");
            return 0;
        }

        resultado = fatorial(A);

        if(resultado == -9999)
        {
            printf("Nao existe fatorial de numero negativo.\n");
            return 0;
        }

        mostraResultado(resultado);

        return 0;
    }

    printf("\nDigite A (-8 ate 7): ");
    scanf("%d",&A);

    printf("Digite B (-8 ate 7): ");
    scanf("%d",&B);

    if(overflow4bits(A) || overflow4bits(B))
    {
        printf("\nEntrada invalida.\n");
        return 0;
    }

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

        default:
            printf("Operacao invalida.\n");
            return 0;
    }

    mostraResultado(resultado);

    return 0;
}

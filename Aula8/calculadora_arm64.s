// calculadora_arm64.s

.global _start
.global soma
.global subtracao
.global multiplicacao
.global fatorial

.text

//--------------------------------------------------
// Ponto de entrada (teste)
//--------------------------------------------------
_start:
    // Teste: soma(10, 20)
    mov w0, #10
    mov w1, #20
    bl soma

    // Resultado fica em w0

fim:
    b fim

//--------------------------------------------------
// int soma(int a, int b)
// a -> w0
// b -> w1
// retorno -> w0
//--------------------------------------------------
soma:
    add w0, w0, w1
    ret

//--------------------------------------------------
// int subtracao(int a, int b)
//--------------------------------------------------
subtracao:
    sub w0, w0, w1
    ret

//--------------------------------------------------
// int multiplicacao(int a, int b)
//--------------------------------------------------
multiplicacao:
    mul w0, w0, w1
    ret

//--------------------------------------------------
// int fatorial(int n)
// n -> w0
// retorno -> w0
//--------------------------------------------------
fatorial:
    mov w1, #1      // resultado
    mov w2, #2      // i

loop:
    cmp w2, w0
    bgt fim_fat

    mul w1, w1, w2
    add w2, w2, #1
    b loop

fim_fat:
    mov w0, w1      // retorno em w0
    ret

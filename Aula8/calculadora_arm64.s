// calculadora_arm64.s

.global soma
.global subtracao
.global multiplicacao
.global fatorial

//--------------------------------------------------
// int soma(int a, int b)
// a -> w0
// b -> w1
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
//--------------------------------------------------
fatorial:

    mov w1, #1          // resultado
    mov w2, #2          // i

loop:

    cmp w2, w0
    bgt fim

    mul w1, w1, w2

    add w2, w2, #1

    b loop

fim:

    mov w0, w1

    ret

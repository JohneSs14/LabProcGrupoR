# calculadora_riscv.s

    .text

    .global _start
    .global soma
    .global subtracao
    .global multiplicacao
    .global fatorial

#--------------------------------------------------
# Ponto de entrada (teste)
#--------------------------------------------------
_start:
    # Teste: soma(10, 20)
    li a0, 10
    li a1, 20
    call soma

    # Resultado fica em a0

fim:
    j fim

#--------------------------------------------------
# int soma(int a, int b)
# a -> a0
# b -> a1
# retorno -> a0
#--------------------------------------------------
soma:
    add a0, a0, a1
    ret

#--------------------------------------------------
# int subtracao(int a, int b)
#--------------------------------------------------
subtracao:
    sub a0, a0, a1
    ret

#--------------------------------------------------
# int multiplicacao(int a, int b)
#--------------------------------------------------
multiplicacao:
    mul a0, a0, a1
    ret

#--------------------------------------------------
# int fatorial(int n)
# n -> a0
# retorno -> a0
#--------------------------------------------------
fatorial:
    li t0, 1          # resultado
    li t1, 2          # i

loop:
    bgt t1, a0, fim_fat

    mul t0, t0, t1
    addi t1, t1, 1

    j loop

fim_fat:
    mv a0, t0         # retorna resultado em a0
    ret

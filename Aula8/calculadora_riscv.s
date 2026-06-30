# calculadora_riscv.s

.globl soma
.globl subtracao
.globl multiplicacao
.globl fatorial

###################################################
# int soma(int a,int b)
# a -> a0
# b -> a1
###################################################
soma:
    add a0, a0, a1
    ret

###################################################
# int subtracao(int a,int b)
###################################################
subtracao:
    sub a0, a0, a1
    ret

###################################################
# int multiplicacao(int a,int b)
###################################################
multiplicacao:
    mul a0, a0, a1
    ret

###################################################
# int fatorial(int n)
###################################################
fatorial:

    li t0, 1          # resultado
    li t1, 2          # i

loop:

    bgt t1, a0, fim

    mul t0, t0, t1

    addi t1, t1, 1

    j loop

fim:

    mv a0, t0

    ret

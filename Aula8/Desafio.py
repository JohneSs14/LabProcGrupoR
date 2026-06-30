#!/usr/bin/env python3

from LCD1602 import CharLCD1602
import Keypad
from time import sleep

ROWS = 4
COLS = 4

keys = [
    '1','2','3','A',
    '4','5','6','B',
    '7','8','9','C',
    '*','0','#','D'
]

rowsPins = [16,20,21,26]
colsPins = [19,13,6,5]

lcd = CharLCD1602()
keypad = Keypad.Keypad(keys, rowsPins, colsPins, ROWS, COLS)
keypad.setDebounceTime(50)

numero1 = ""
numero2 = ""
operacao = ""

def limpar():
    global numero1, numero2, operacao

    numero1 = ""
    numero2 = ""
    operacao = ""

    lcd.clear()
    lcd.write(0,0,"Calculadora")
    lcd.write(0,1,"A:+ B:- C:*")


def calcular():

    global numero1
    global numero2
    global operacao

    if numero1 == "" or operacao == "":
        return

    a = int(numero1)

    if operacao != "F":
        if numero2 == "":
            return
        b = int(numero2)

    try:

        if operacao == 'A':
            resultado = a + b

        elif operacao == 'B':
            resultado = a - b

        elif operacao == 'C':
            resultado = a * b

        elif operacao == 'D':

            if b == 0:

                lcd.clear()
                lcd.write(0,0,"ERRO")
                lcd.write(0,1,"DIV POR ZERO")

                sleep(2)

                limpar()
                return

            resultado = a // b

        elif operacao == 'F':

            resultado = 1

            for i in range(2,a+1):
                resultado *= i

        lcd.clear()

        lcd.write(0,0,"Resultado")

        lcd.write(0,1,str(resultado))

        sleep(4)

        limpar()

    except Exception:

        lcd.clear()

        lcd.write(0,0,"Erro")

        sleep(2)

        limpar()


def loop():

    global numero1
    global numero2
    global operacao

    limpar()

    while True:

        tecla = keypad.getKey()

        if tecla == keypad.NULL:
            continue

        ##########################################

        if tecla == '*':
            limpar()
            continue

        ##########################################

        if tecla == '#':
            calcular()
            continue

        ##########################################

        if tecla in ['A','B','C','D']:

            operacao = tecla

            lcd.write(0,1,tecla)

            continue

        ##########################################

        if tecla == 'F':

            operacao = "F"

            continue

        ##########################################

        if tecla.isdigit():

            if operacao == "":

                numero1 += tecla

            else:

                numero2 += tecla

            lcd.clear()

            texto = numero1

            if operacao != "":
                texto += operacao

            texto += numero2

            lcd.write(0,0,texto)


try:

    loop()

except KeyboardInterrupt:

    lcd.clear()

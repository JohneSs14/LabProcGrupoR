#!/usr/bin/env python3

import time
import Keypad
from LCD1602 import CharLCD1602

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
lcd.init_lcd()

keypad = Keypad.Keypad(keys, rowsPins, colsPins, ROWS, COLS)
keypad.setDebounceTime(50)

numero1 = ""
numero2 = ""
operacao = None


def atualizarDisplay():
    lcd.clear()

    linha = numero1

    if operacao is not None:
        linha += operacao

    linha += numero2

    lcd.write(0,0,linha[:16])

    lcd.write(0,1,"#=OK *=CLR")


def limpar():
    global numero1
    global numero2
    global operacao

    numero1 = ""
    numero2 = ""
    operacao = None

    atualizarDisplay()


def calcular():

    global numero1
    global numero2
    global operacao

    if numero1 == "" or numero2 == "" or operacao is None:
        return

    a = int(numero1)
    b = int(numero2)

    try:

        if operacao == "+":
            r = a+b

        elif operacao == "-":
            r = a-b

        elif operacao == "*":
            r = a*b

        elif operacao == "/":

            if b == 0:
                lcd.clear()
                lcd.write(0,0,"ERRO")
                lcd.write(0,1,"Divisao Zero")
                time.sleep(2)
                limpar()
                return

            r = a//b

        lcd.clear()
        lcd.write(0,0,"Resultado:")
        lcd.write(0,1,str(r)[:16])

        time.sleep(3)

    except:

        lcd.clear()
        lcd.write(0,0,"Erro")

        time.sleep(2)

    limpar()


limpar()

while True:

    tecla = keypad.getKey()

    if tecla == keypad.NULL:
        continue

    print(tecla)

    if tecla == "*":
        limpar()
        continue

    if tecla == "#":
        calcular()
        continue

    if tecla == "A":
        operacao = "+"
        atualizarDisplay()
        continue

    if tecla == "B":
        operacao = "-"
        atualizarDisplay()
        continue

    if tecla == "C":
        operacao = "*"
        atualizarDisplay()
        continue

    if tecla == "D":
        operacao = "/"
        atualizarDisplay()
        continue

    if tecla.isdigit():

        if operacao is None:
            numero1 += tecla
        else:
            numero2 += tecla

        atualizarDisplay()

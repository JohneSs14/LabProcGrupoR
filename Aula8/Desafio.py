#!/usr/bin/env python3

from gpiozero import OutputDevice, InputDevice
from LCD1602 import CharLCD1602
from time import sleep

# ---------------- LCD ----------------

lcd = CharLCD1602()
lcd.init_lcd()

# ---------------- TECLADO ----------------

ROWS = [16,20,21,26]
COLS = [19,13,6,5]

KEYS = [
    ['1','2','3','A'],
    ['4','5','6','B'],
    ['7','8','9','C'],
    ['*','0','#','D']
]

rows = [OutputDevice(pin) for pin in ROWS]
cols = [InputDevice(pin,pull_up=True) for pin in COLS]

for r in rows:
    r.on()

# ---------------- CALCULADORA ----------------

numero1=""
numero2=""
operacao=None

def mostrar():

    lcd.clear()

    texto = numero1

    if operacao:
        texto += operacao

    texto += numero2

    lcd.write(0,0,texto[:16])
    lcd.write(0,1,"#=* OK")


def limpar():

    global numero1
    global numero2
    global operacao

    numero1=""
    numero2=""
    operacao=None

    mostrar()


def calcular():

    global numero1
    global numero2
    global operacao

    if numero1=="" or numero2=="" or operacao is None:
        return

    a=int(numero1)
    b=int(numero2)

    try:

        if operacao=="+":
            r=a+b

        elif operacao=="-":
            r=a-b

        elif operacao=="*":
            r=a*b

        elif operacao=="/":

            if b==0:

                lcd.clear()
                lcd.write(0,0,"ERRO")
                lcd.write(0,1,"DIV POR ZERO")
                sleep(2)
                limpar()
                return

            r=a//b

        lcd.clear()
        lcd.write(0,0,"Resultado")
        lcd.write(0,1,str(r)[:16])

        sleep(3)

    except:

        lcd.clear()
        lcd.write(0,0,"ERRO")
        sleep(2)

    limpar()


def lerTecla():

    while True:

        for i,row in enumerate(rows):

            for r in rows:
                r.on()

            row.off()

            for j,col in enumerate(cols):

                if col.value == 0:

                    sleep(0.2)

                    while col.value==0:
                        pass

                    return KEYS[i][j]


limpar()

while True:

    tecla=lerTecla()

    print(tecla)

    if tecla=="*":

        limpar()

    elif tecla=="#":

        calcular()

    elif tecla=="A":

        operacao="+"
        mostrar()

    elif tecla=="B":

        operacao="-"
        mostrar()

    elif tecla=="C":

        operacao="*"
        mostrar()

    elif tecla=="D":

        operacao="/"
        mostrar()

    else:

        if operacao is None:

            numero1+=tecla

        else:

            numero2+=tecla

        mostrar()

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

cajote@raspberry-pi-labproc:~/Downloads $ python3 Desafio.py
Traceback (most recent call last):
  File "/usr/lib/python3/dist-packages/gpiozero/pins/pi.py", line 411, in pin
    pin = self.pins[info]
          ~~~~~~~~~^^^^^^
KeyError: PinInfo(number=36, name='GPIO16', names=frozenset({16, 'J8:36', '16', 'GPIO16', 'BOARD36', 'BCM16', 'WPI27'}), pull='', row=18, col=2, interfaces=frozenset({'', 'gpio', 'uart', 'dpi', 'spi'}))

During handling of the above exception, another exception occurred:

Traceback (most recent call last):
  File "/home/cajote/Downloads/Desafio.py", line 113, in <module>
    tecla = keypad.getKey()
  File "/home/cajote/Downloads/Keypad.py", line 49, in getKey
    if(self.getKeys() and self.key[0].stateChanged and (self.key[0].kstate == self.key[0].PRESSED)):
       ~~~~~~~~~~~~^^
  File "/home/cajote/Downloads/Keypad.py", line 58, in getKeys
    self.scanKeys()
    ~~~~~~~~~~~~~^^
  File "/home/cajote/Downloads/Keypad.py", line 65, in scanKeys
    inputs = list(map(lambda pin: InputDevice(pin, pull_up=True), self.rowPins))
  File "/home/cajote/Downloads/Keypad.py", line 65, in <lambda>
    inputs = list(map(lambda pin: InputDevice(pin, pull_up=True), self.rowPins))
                                  ~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^
  File "/usr/lib/python3/dist-packages/gpiozero/devices.py", line 108, in __call__
    self = super().__call__(*args, **kwargs)
  File "/usr/lib/python3/dist-packages/gpiozero/input_devices.py", line 79, in __init__
    super().__init__(pin, pin_factory=pin_factory)
    ~~~~~~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/usr/lib/python3/dist-packages/gpiozero/devices.py", line 553, in __init__
    pin = self.pin_factory.pin(pin)
  File "/usr/lib/python3/dist-packages/gpiozero/pins/pi.py", line 413, in pin
    pin = self.pin_class(self, info)
  File "/usr/lib/python3/dist-packages/gpiozero/pins/lgpio.py", line 126, in __init__
    lgpio.gpio_claim_input(
    ~~~~~~~~~~~~~~~~~~~~~~^
        self.factory._handle, self._number, lgpio.SET_PULL_NONE)
        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/usr/lib/python3/dist-packages/lgpio.py", line 755, in gpio_claim_input
    return _u2i(_lgpio._gpio_claim_input(handle&0xffff, lFlags, gpio))
  File "/usr/lib/python3/dist-packages/lgpio.py", line 458, in _u2i
    raise error(error_text(v))
lgpio.error: 'GPIO busy'

#!/usr/bin/env python3
import Keypad

ROWS = 4
COLS = 4
keys = ['1','4','7','*',
        '2','5','8','0',
        '3','6','9','#',
        'A','B','C','D']
rowsPins = [19, 13,  6,  5]
colsPins  = [16, 20, 21, 26]

keypad = Keypad.Keypad(keys, rowsPins, colsPins, ROWS, COLS)
keypad.setDebounceTime(50)

print("Aguardando teclas... (Ctrl+C para sair)")
try:
    while True:
        key = keypad.getKey()
        if key != keypad.NULL:
            print(f"Tecla pressionada: {key}")
except KeyboardInterrupt:
    print("Encerrando.")

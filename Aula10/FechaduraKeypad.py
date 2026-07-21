#!/usr/bin/env python3
import Keypad

ROWS = 4
COLS = 4
keys = ['1','2','3','A',
        '4','5','6','B',
        '7','8','9','C',
        '*','0','#','D']
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

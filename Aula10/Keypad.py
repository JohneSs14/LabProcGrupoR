#!/usr/bin/env python3
# Modulo Keypad usando gpiozero (sem conflito com outros componentes)
from gpiozero import DigitalOutputDevice, DigitalInputDevice
from time import sleep

class Keypad:
    NULL = None

    def __init__(self, usrKeyMap, row_Pins, col_Pins, num_Rows, num_Cols):
        self.keyMap  = usrKeyMap
        self.numRows = num_Rows
        self.numCols = num_Cols
        self.debounce = 0.05

        self.rows = [DigitalOutputDevice(p, active_high=False, initial_value=False) for p in row_Pins]
        self.cols = [DigitalInputDevice(p, pull_up=True) for p in col_Pins]

        # Desativa todas as linhas inicialmente
        for r in self.rows:
            r.off()

    def setDebounceTime(self, ms):
        self.debounce = ms / 1000.0

    def getKey(self):
        for r_idx, row in enumerate(self.rows):
            row.on()   # ativa linha (puxa LOW via active_high=False)
            for c_idx, col in enumerate(self.cols):
                if not col.value:  # coluna foi puxada LOW
                    sleep(self.debounce)
                    if not col.value:
                        while not col.value:
                            pass
                        row.off()
                        return self.keyMap[r_idx * self.numCols + c_idx]
            row.off()
        return self.NULL

    def cleanup(self):
        for r in self.rows:
            r.close()
        for c in self.cols:
            c.close()

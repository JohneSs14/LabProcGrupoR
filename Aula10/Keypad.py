#!/usr/bin/env python3
# Implementacao simplificada do modulo Keypad para teclado matricial 4x4
# Compativel com a interface usada pelo kit Freenove
import RPi.GPIO as GPIO
import time

class Keypad:
    NULL = None

    def __init__(self, usrKeyMap, row_Pins, col_Pins, num_Rows, num_Cols):
        self.keyMap   = usrKeyMap
        self.rowPins  = row_Pins
        self.colPins  = col_Pins
        self.numRows  = num_Rows
        self.numCols  = num_Cols
        self.debounce = 0.05  # 50ms default

        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)

        for pin in self.rowPins:
            GPIO.setup(pin, GPIO.OUT, initial=GPIO.HIGH)
        for pin in self.colPins:
            GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

    def setDebounceTime(self, ms):
        self.debounce = ms / 1000.0

    def getKey(self):
        for r, rowPin in enumerate(self.rowPins):
            GPIO.output(rowPin, GPIO.LOW)
            for c, colPin in enumerate(self.colPins):
                if GPIO.input(colPin) == GPIO.LOW:
                    time.sleep(self.debounce)
                    if GPIO.input(colPin) == GPIO.LOW:
                        while GPIO.input(colPin) == GPIO.LOW:
                            pass
                        GPIO.output(rowPin, GPIO.HIGH)
                        return self.keyMap[r * self.numCols + c]
            GPIO.output(rowPin, GPIO.HIGH)
        return self.NULL

    def cleanup(self):
        GPIO.cleanup()

#!/usr/bin/env python3

import RPi.GPIO as GPIO
import time

TRIG = 14          # GPIO BCM
ECHO = 15          # GPIO BCM
LIMIAR_CM = 5.0

GPIO.setmode(GPIO.BCM)

GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

GPIO.output(TRIG, False)
time.sleep(2)

print("Monitorando HC-SR04... (Ctrl+C para sair)\n")

try:
    while True:

        # Pulso de 10 us
        GPIO.output(TRIG, True)
        time.sleep(0.00001)
        GPIO.output(TRIG, False)

        # Espera o início do eco
        timeout = time.time() + 0.05
        while GPIO.input(ECHO) == 0:
            inicio = time.time()
            if time.time() > timeout:
                raise TimeoutError("Timeout esperando início do eco.")

        # Espera o fim do eco
        timeout = time.time() + 0.05
        while GPIO.input(ECHO) == 1:
            fim = time.time()
            if time.time() > timeout:
                raise TimeoutError("Timeout esperando fim do eco.")

        duracao = fim - inicio

        distancia = duracao * 34300 / 2

        estado = "TRAVADA" if distancia < LIMIAR_CM else "ABERTA"

        print(f"Distância: {distancia:5.1f} cm | Fechadura: {estado}")

        time.sleep(0.5)

except KeyboardInterrupt:
    print("\nEncerrando...")

except TimeoutError as e:
    print(e)

finally:
    GPIO.cleanup()

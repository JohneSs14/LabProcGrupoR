#!/usr/bin/env python3
import RPi.GPIO as GPIO
from time import sleep, time

TRIG_PIN  = 14
ECHO_PIN  = 15
LIMIAR_CM = 5.0

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(TRIG_PIN, GPIO.OUT)
GPIO.setup(ECHO_PIN, GPIO.IN)

print(f"Estado inicial do ECHO (GPIO{ECHO_PIN}): {GPIO.input(ECHO_PIN)} (esperado 0)")

def medir_distancia():
    GPIO.output(TRIG_PIN, False)
    sleep(0.05)
    GPIO.output(TRIG_PIN, True)
    sleep(0.00001)
    GPIO.output(TRIG_PIN, False)

    timeout = time() + 0.5
    while GPIO.input(ECHO_PIN) == 0:
        if time() > timeout:
            return -1, "ECHO nunca subiu para HIGH"
    inicio = time()

    timeout = time() + 0.5
    while GPIO.input(ECHO_PIN) == 1:
        if time() > timeout:
            return -2, "ECHO ficou preso em HIGH"
    fim = time()

    distancia = (fim - inicio) * 34300 / 2
    return distancia, "OK"

print("Monitorando... (Ctrl+C para sair)")
print(f"Limiar de travamento: {LIMIAR_CM} cm\n")
try:
    while True:
        dist, status = medir_distancia()
        if dist < 0:
            print(f"Sem resposta: {status}")
        else:
            estado = "TRAVADA" if dist < LIMIAR_CM else "ABERTA "
            print(f"Distancia: {dist:5.1f} cm  |  Fechadura: {estado}")
        sleep(1)
except KeyboardInterrupt:
    print("Encerrando.")
finally:
    GPIO.cleanup()

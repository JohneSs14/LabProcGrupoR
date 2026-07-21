#!/usr/bin/env python3
# Diagnostico: descobre qual pino fisico corresponde a qual tecla
import RPi.GPIO as GPIO
from time import sleep

TODOS_PINOS = [16, 20, 21, 26, 19, 13, 6, 5]

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

print("=== DIAGNOSTICO DO TECLADO ===")
print("Pressione e SEGURE uma tecla quando solicitado.\n")

for pino_saida in TODOS_PINOS:
    # Configura todos como entrada pull-up
    for p in TODOS_PINOS:
        GPIO.setup(p, GPIO.IN, pull_up_down=GPIO.PUD_UP)

    # Coloca um pino como saida LOW
    GPIO.setup(pino_saida, GPIO.OUT)
    GPIO.output(pino_saida, GPIO.LOW)

    print(f"GPIO {pino_saida} em LOW — pressione uma tecla na linha/coluna correspondente:")
    sleep(2)

    for pino_entrada in TODOS_PINOS:
        if pino_entrada == pino_saida:
            continue
        if GPIO.input(pino_entrada) == GPIO.LOW:
            print(f"  -> GPIO {pino_entrada} detectou LOW (tecla pressionada!)")

    GPIO.setup(pino_saida, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    print()

GPIO.cleanup()
print("Diagnostico concluido.")

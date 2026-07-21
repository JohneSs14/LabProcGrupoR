#!/usr/bin/env python3
from gpiozero import DistanceSensor
from gpiozero.pins.pigpio import PiGPIOFactory
from time import sleep
import warnings
warnings.filterwarnings("ignore")

TRIG_PIN  = 14
ECHO_PIN  = 15
LIMIAR_CM = 5.0

factory = PiGPIOFactory()
sensor = DistanceSensor(echo=ECHO_PIN, trigger=TRIG_PIN, max_distance=3, pin_factory=factory)

print("Monitorando sensor ultrassonico... (Ctrl+C para sair)")
print(f"Limiar de travamento: {LIMIAR_CM} cm\n")
try:
    while True:
        dist = sensor.distance * 100
        estado = "TRAVADA" if dist < LIMIAR_CM else "ABERTA "
        print(f"Distancia: {dist:5.1f} cm  |  Fechadura: {estado}")
        sleep(1)
except KeyboardInterrupt:
    print("Encerrando.")
finally:
    sensor.close()

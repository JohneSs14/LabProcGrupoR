#!/usr/bin/env python3
from gpiozero import TonalBuzzer
from gpiozero.tones import Tone
from time import sleep

buzzer = TonalBuzzer(4)

def bip_sucesso():
    for nota in ["C5", "E5", "G5"]:
        buzzer.play(Tone(nota))
        sleep(0.15)
    buzzer.stop()

def bip_erro():
    buzzer.play(Tone("A3"))
    sleep(0.5)
    buzzer.stop()

print("Testando bip de SUCESSO:")
bip_sucesso()
sleep(1)

print("Testando bip de ERRO:")
bip_erro()
sleep(1)

buzzer.close()
print("Teste concluido.")

from gpiozero import Button
from gpiozero import AngularServo
from gpiozero import TonalBuzzer
from gpiozero.tones import Tone
from time import sleep
import time

servo = AngularServo(18)
buzzer = TonalBuzzer(4)

mais = Button(24)
menos = Button(25)

bpm = 60

estado = False

while True:

    if mais.is_pressed:
        bpm += 5
        sleep(0.2)

    if menos.is_pressed:
        bpm = max(20,bpm-5)
        sleep(0.2)

    periodo = 60/bpm

    servo.angle = 45 if estado else -45

    buzzer.play(Tone("A5"))
    sleep(0.08)
    buzzer.stop()

    estado = not estado

    sleep(periodo-0.08)

from gpiozero import AngularServo
from gpiozero import TonalBuzzer
from gpiozero.tones import Tone
from time import sleep

servo = AngularServo(18)
buzzer = TonalBuzzer(4)

estado = False

while True:

    if estado:
        servo.angle = -45
    else:
        servo.angle = 45

    buzzer.play(Tone("A5"))
    sleep(0.10)

    buzzer.stop()

    estado = not estado

    sleep(0.90)

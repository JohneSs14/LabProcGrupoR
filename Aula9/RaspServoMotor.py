from gpiozero import AngularServo
from time import sleep

SERVO_PIN = 17

servo = AngularServo(
    SERVO_PIN,
    min_angle=-90,
    max_angle=90
)

while True:

    servo.angle = -90
    sleep(2)

    servo.angle = 0
    sleep(2)

    servo.angle = 90
    sleep(2)

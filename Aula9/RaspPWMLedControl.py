from gpiozero import PWMLED
from time import sleep

LED_PIN = 18

led = PWMLED(LED_PIN)

frequencias = [1, 5, 10, 50, 100, 500]

for freq in frequencias:

    print(f"\nTestando {freq} Hz")

    led.frequency = freq

    for brilho in [0,0.25,0.5,0.75,1]:
        led.value = brilho
        sleep(2)

led.off()

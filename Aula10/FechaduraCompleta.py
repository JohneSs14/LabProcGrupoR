#!/usr/bin/env python3
import Keypad
from gpiozero import AngularServo, TonalBuzzer, DistanceSensor
from gpiozero.tones import Tone
from LCD1602 import CharLCD1602
from time import sleep
import warnings
warnings.filterwarnings("ignore")

# ── Configurações ──────────────────────────────────────────────
SENHA_CORRETA  = "1234"
LIMIAR_CM      = 5.0

TRIG_PIN       = 23
ECHO_PIN       = 24
SERVO_PIN      = 18
BUZZER_PIN     = 4

ROWS = 4
COLS = 4
keys = ['1','4','7','*',
        '2','5','8','0',
        '3','6','9','#',
        'A','B','C','D']
rowsPins = [19, 13,  6,  5]
colsPins  = [16, 20, 21, 26]

# ── Inicialização dos componentes ──────────────────────────────
keypad = Keypad.Keypad(keys, rowsPins, colsPins, ROWS, COLS)
keypad.setDebounceTime(50)

lcd    = CharLCD1602()
lcd.init_lcd()

buzzer  = TonalBuzzer(BUZZER_PIN)
servo   = AngularServo(SERVO_PIN)
sensor  = DistanceSensor(echo=ECHO_PIN, trigger=TRIG_PIN, max_distance=3)

# ── Funções auxiliares ─────────────────────────────────────────
def bip_sucesso():
    for nota in ["C5", "E5", "G5"]:
        buzzer.play(Tone(nota))
        sleep(0.15)
    buzzer.stop()

def bip_erro():
    buzzer.play(Tone("A3"))
    sleep(0.5)
    buzzer.stop()

def abrir_fechadura():
    print("[SERVO] Abrindo...")
    servo.angle = 90
    sleep(3)
    servo.angle = 0
    print("[SERVO] Travado novamente.")

def travar_fechadura():
    servo.angle = 0

def atualizar_lcd_entrada(entrada):
    lcd.clear()
    lcd.write(0, 0, 'Digite a senha:')
    mascara = '*' * len(entrada) + '_' * (4 - len(entrada))
    lcd.write(0, 1, mascara)

def mostrar_mensagem(linha1, linha2='', duracao=2):
    lcd.clear()
    lcd.write(0, 0, linha1[:16])
    if linha2:
        lcd.write(0, 1, linha2[:16])
    sleep(duracao)

def verificar_sensor():
    dist = sensor.distance * 100
    estado = "TRAVADA" if dist < LIMIAR_CM else "ABERTA"
    print(f"[SENSOR] Distancia: {dist:.1f} cm | Fechadura: {estado}")

# ── Loop principal ─────────────────────────────────────────────
travar_fechadura()
entrada = ""
atualizar_lcd_entrada(entrada)
print("Sistema pronto. Senha padrao: 1234")
print("* = limpar  |  # = confirmar\n")

try:
    ciclo = 0
    while True:
        ciclo += 1
        if ciclo % 10 == 0:
            verificar_sensor()

        key = keypad.getKey()
        if key == keypad.NULL:
            sleep(0.05)
            continue

        print(f"[KEYPAD] Tecla: {key}")

        if key == '*':
            entrada = ""
            atualizar_lcd_entrada(entrada)

        elif key == '#':
            if entrada == SENHA_CORRETA:
                print("[OK] Senha correta!")
                mostrar_mensagem('ACESSO LIBERADO', 'Bem-vindo!', 1)
                bip_sucesso()
                abrir_fechadura()
            else:
                print("[ERRO] Senha incorreta!")
                mostrar_mensagem('SENHA INCORRETA', 'Tente novamente', 2)
                bip_erro()
            entrada = ""
            atualizar_lcd_entrada(entrada)

        elif key.isdigit() and len(entrada) < 4:
            entrada += key
            atualizar_lcd_entrada(entrada)

except KeyboardInterrupt:
    print("\nSistema encerrado.")
finally:
    lcd.clear()
    buzzer.close()
    sensor.close()
    servo.close()

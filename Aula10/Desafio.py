#!/usr/bin/env python3
# Desafio: Fechadura com protecao contra forca bruta e log de tentativas
import Keypad
import RPi.GPIO as GPIO
from LCD1602 import CharLCD1602
from time import sleep, time
from datetime import datetime

# ── Configurações ──────────────────────────────────────────────
SENHA_CORRETA  = "1234"
MAX_TENTATIVAS = 3
TEMPO_BLOQUEIO = 10
LIMIAR_CM      = 5.0
LOG_FILE       = "tentativas.log"

TRIG_PIN   = 14
ECHO_PIN   = 15
SERVO_PIN  = 18
BUZZER_PIN = 4

ROWS = 4
COLS = 4
keys = ['1','4','7','*',
        '2','5','8','0',
        '3','6','9','#',
        'A','B','C','D']
rowsPins = [19, 13,  6,  5]
colsPins  = [16, 20, 21, 26]

SERVO_TRAVADO = 2.5
SERVO_ABERTO  = 7.5
NOTAS = {"C5": 523, "E5": 659, "G5": 784, "A3": 220}

# ── Inicialização GPIO ─────────────────────────────────────────
keypad = Keypad.Keypad(keys, rowsPins, colsPins, ROWS, COLS)
keypad.setDebounceTime(50)

GPIO.setup(TRIG_PIN,   GPIO.OUT)
GPIO.setup(ECHO_PIN,   GPIO.IN)
GPIO.setup(SERVO_PIN,  GPIO.OUT)
GPIO.setup(BUZZER_PIN, GPIO.OUT)

servo_pwm  = GPIO.PWM(SERVO_PIN,  50)
buzzer_pwm = GPIO.PWM(BUZZER_PIN, 440)
servo_pwm.start(SERVO_TRAVADO)
buzzer_pwm.start(0)

lcd = CharLCD1602()
lcd.init_lcd()

# ── Estado global ──────────────────────────────────────────────
tentativas_erradas = 0
bloqueada = False

# ── Sensor ─────────────────────────────────────────────────────
def medir_distancia():
    GPIO.output(TRIG_PIN, False)
    sleep(0.05)
    GPIO.output(TRIG_PIN, True)
    sleep(0.00001)
    GPIO.output(TRIG_PIN, False)
    t = time()
    while GPIO.input(ECHO_PIN) == 0:
        if time() - t > 0.3:
            return -1
    inicio = time()
    while GPIO.input(ECHO_PIN) == 1:
        if time() - inicio > 0.3:
            return -1
    return (time() - inicio) * 34300 / 2

# ── Servo ──────────────────────────────────────────────────────
def set_servo(duty):
    servo_pwm.ChangeDutyCycle(duty)
    sleep(0.5)
    servo_pwm.ChangeDutyCycle(0)

# ── Buzzer ─────────────────────────────────────────────────────
def tocar_nota(nota_str, duracao):
    freq = NOTAS.get(nota_str, 440)
    buzzer_pwm.ChangeFrequency(freq)
    buzzer_pwm.ChangeDutyCycle(50)
    sleep(duracao)
    buzzer_pwm.ChangeDutyCycle(0)

def bip_sucesso():
    for nota in ["C5", "E5", "G5"]:
        tocar_nota(nota, 0.15)

def bip_erro():
    tocar_nota("A3", 0.5)

def bip_bloqueio():
    for _ in range(3):
        tocar_nota("A3", 0.2)
        sleep(0.1)

# ── Log ────────────────────────────────────────────────────────
def registrar_log(evento):
    ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    linha = f"[{ts}] {evento}"
    print(linha)
    with open(LOG_FILE, "a") as f:
        f.write(linha + "\n")

# ── LCD ────────────────────────────────────────────────────────
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

# ── Fechadura ──────────────────────────────────────────────────
def abrir_fechadura():
    set_servo(SERVO_ABERTO)
    sleep(3)
    set_servo(SERVO_TRAVADO)

def bloquear_sistema():
    global bloqueada, tentativas_erradas
    bloqueada = True
    registrar_log("ALERTA: sistema bloqueado por forca bruta")
    bip_bloqueio()
    for seg in range(TEMPO_BLOQUEIO, 0, -1):
        lcd.clear()
        lcd.write(0, 0, 'SISTEMA BLOQUEADO')
        lcd.write(0, 1, f'Aguarde {seg:2d}s...')
        sleep(1)
    bloqueada = False
    tentativas_erradas = 0
    registrar_log("Sistema desbloqueado.")

def verificar_sensor():
    dist = medir_distancia()
    if dist < 0:
        registrar_log("[SENSOR] Sem resposta")
    else:
        estado = "TRAVADA" if dist < LIMIAR_CM else "ABERTA"
        registrar_log(f"[SENSOR] {dist:.1f} cm | {estado}")

# ── Loop principal ─────────────────────────────────────────────
set_servo(SERVO_TRAVADO)
entrada = ""
atualizar_lcd_entrada(entrada)
registrar_log("Sistema iniciado.")
print(f"Senha padrao: {SENHA_CORRETA} | Bloqueio apos {MAX_TENTATIVAS} erros\n")

try:
    ciclo = 0
    while True:
        ciclo += 1
        if ciclo % 60 == 0:
            verificar_sensor()

        if bloqueada:
            sleep(0.1)
            continue

        key = keypad.getKey()
        if key == keypad.NULL:
            sleep(0.05)
            continue

        if key == '*':
            entrada = ""
            atualizar_lcd_entrada(entrada)

        elif key == '#':
            if entrada == SENHA_CORRETA:
                tentativas_erradas = 0
                registrar_log("Acesso CONCEDIDO.")
                mostrar_mensagem('ACESSO LIBERADO', 'Bem-vindo!', 1)
                bip_sucesso()
                abrir_fechadura()
            else:
                tentativas_erradas += 1
                registrar_log(f"Acesso NEGADO (tentativa {tentativas_erradas}/{MAX_TENTATIVAS})")
                if tentativas_erradas >= MAX_TENTATIVAS:
                    bloquear_sistema()
                else:
                    restantes = MAX_TENTATIVAS - tentativas_erradas
                    mostrar_mensagem('SENHA INCORRETA', f'Restam {restantes} tent.', 2)
                    bip_erro()
            entrada = ""
            atualizar_lcd_entrada(entrada)

        elif key.isdigit() and len(entrada) < 4:
            entrada += key
            atualizar_lcd_entrada(entrada)

except KeyboardInterrupt:
    registrar_log("Sistema encerrado pelo usuario.")
finally:
    lcd.clear()
    servo_pwm.stop()
    buzzer_pwm.stop()
    GPIO.cleanup()

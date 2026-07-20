#!/usr/bin/env python3
# Desafio: Fechadura com protecao contra forca bruta e log de tentativas
import sys
sys.path.append('/home/pi/Freenove_Kit/Code/Python_GPIOZero_Code/21_MatrixKeypad')
import Keypad
from gpiozero import AngularServo, TonalBuzzer, DistanceSensor
from gpiozero.tones import Tone
from LCD1602 import CharLCD1602
from time import sleep
from datetime import datetime
import warnings
warnings.filterwarnings("ignore")

# ── Configurações ──────────────────────────────────────────────
SENHA_CORRETA     = "1234"
MAX_TENTATIVAS    = 3       # bloqueia após N erros consecutivos
TEMPO_BLOQUEIO    = 10      # segundos bloqueada após exceder tentativas
LIMIAR_CM         = 5.0
LOG_FILE          = "tentativas.log"

TRIG_PIN  = 14
ECHO_PIN  = 15
SERVO_PIN = 18
BUZZER_PIN = 4

ROWS = 4
COLS = 4
keys = ['1','2','3','A',
        '4','5','6','B',
        '7','8','9','C',
        '*','0','#','D']
rowsPins = [16, 20, 21, 26]
colsPins  = [19, 13,  6,  5]

# ── Inicialização ──────────────────────────────────────────────
keypad = Keypad.Keypad(keys, rowsPins, colsPins, ROWS, COLS)
keypad.setDebounceTime(50)

lcd    = CharLCD1602()
lcd.init_lcd()

buzzer = TonalBuzzer(BUZZER_PIN)
servo  = AngularServo(SERVO_PIN)
sensor = DistanceSensor(echo=ECHO_PIN, trigger=TRIG_PIN, max_distance=3)

# ── Estado global ──────────────────────────────────────────────
tentativas_erradas = 0
bloqueada = False

# ── Funções de log ─────────────────────────────────────────────
def registrar_log(evento):
    ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    linha = f"[{ts}] {evento}"
    print(linha)
    with open(LOG_FILE, "a") as f:
        f.write(linha + "\n")

# ── Funções de feedback ────────────────────────────────────────
def bip_sucesso():
    for nota in ["C5", "E5", "G5"]:
        buzzer.play(Tone(nota))
        sleep(0.15)
    buzzer.stop()

def bip_erro():
    buzzer.play(Tone("A3"))
    sleep(0.5)
    buzzer.stop()

def bip_bloqueio():
    for _ in range(3):
        buzzer.play(Tone("A3"))
        sleep(0.2)
        buzzer.stop()
        sleep(0.1)

def abrir_fechadura():
    servo.angle = 90
    sleep(3)
    servo.angle = 0

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

# ── Loop principal ─────────────────────────────────────────────
travar_fechadura()
entrada = ""
atualizar_lcd_entrada(entrada)
registrar_log("Sistema iniciado.")
print(f"Senha padrao: {SENHA_CORRETA} | Bloqueio apos {MAX_TENTATIVAS} erros\n")

try:
    while True:
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
                    mostrar_mensagem('SENHA INCORRETA', f'Restam {restantes} tentativas', 2)
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
    buzzer.close()
    sensor.close()
    servo.close()

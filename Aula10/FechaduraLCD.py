#!/usr/bin/env python3
from LCD1602 import CharLCD1602
from time import sleep

lcd = CharLCD1602()
lcd.init_lcd()

print("Teste 1: mensagem de boas-vindas")
lcd.write(0, 0, 'Fechadura')
lcd.write(0, 1, 'Eletronica v1.0')
sleep(3)

print("Teste 2: tela de entrada de senha")
lcd.clear()
lcd.write(0, 0, 'Digite a senha:')
lcd.write(0, 1, '____')
sleep(3)

print("Teste 3: acesso liberado")
lcd.clear()
lcd.write(0, 0, 'ACESSO LIBERADO')
lcd.write(0, 1, 'Bem-vindo!')
sleep(3)

print("Teste 4: senha incorreta")
lcd.clear()
lcd.write(0, 0, 'SENHA INCORRETA')
lcd.write(0, 1, 'Tente novamente')
sleep(3)

lcd.clear()
print("Teste concluido.")

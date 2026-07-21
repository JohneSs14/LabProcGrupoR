#!/bin/bash
# Script para instalar pigpio no Raspberry Pi
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
sudo pigpiod
echo "pigpio instalado e iniciado com sucesso!"

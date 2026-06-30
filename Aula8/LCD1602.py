#!/usr/bin/env python3
import time
import smbus
import subprocess

class CharLCD1602(object):
    def __init__(self):
        # Note you need to change the bus number to 0 if running on a revision 1 Raspberry Pi.
        self.bus = smbus.SMBus(1)
        self.BLEN = 1  # turn on/off background light
        self.PCF8574_address = 0x27  # I2C address of the PCF8574 chip.
        self.PCF8574A_address = 0x3f  # I2C address of the PCF8574A chip.
        self.LCD_ADDR =self.PCF8574_address  
    def write_word(self,addr, data):
        temp = data
        if self.BLEN == 1:
            temp |= 0x08
        else:
            temp &= 0xF7
        self.bus.write_byte(addr ,temp)

    def send_command(self,comm):
        # Send bit7-4 firstly
        buf = comm & 0xF0
        buf |= 0x04               # RS = 0, RW = 0, EN = 1
        self.write_word(self.LCD_ADDR ,buf)
        time.sleep(0.002)
        buf &= 0xFB               # Make EN = 0
        self.write_word(self.LCD_ADDR ,buf)
        # Send bit3-0 secondly
        buf = (comm & 0x0F) << 4
        buf |= 0x04               # RS = 0, RW = 0, EN = 1
        self.write_word(self.LCD_ADDR ,buf)
        time.sleep(0.002)
        buf &= 0xFB               # Make EN = 0
        self.write_word(self.LCD_ADDR ,buf)

    def send_data(self,data):
        # Send bit7-4 firstly
        buf = data & 0xF0
        buf |= 0x05               # RS = 1, RW = 0, EN = 1
        self.write_word(self.LCD_ADDR ,buf)
        time.sleep(0.002)
        buf &= 0xFB               # Make EN = 0
        self.write_word(self.LCD_ADDR ,buf)
        # Send bit3-0 secondly
        buf = (data & 0x0F) << 4
        buf |= 0x05               # RS = 1, RW = 0, EN = 1
        self.write_word(self.LCD_ADDR ,buf)
        time.sleep(0.002)
        buf &= 0xFB               # Make EN = 0
        self.write_word(self.LCD_ADDR ,buf)

    def i2c_scan(self):
        cmd = "i2cdetect -y 1 |awk \'NR>1 {$1=\"\";print}\'"
        result = subprocess.check_output(cmd, shell=True).decode()
        result = result.replace("\n", "").replace(" --", "")
        i2c_list = result.split(' ')
        return i2c_list

    def init_lcd(self,addr=None, bl=1):
        i2c_list = self.i2c_scan()
#         print(f"i2c_list: {i2c_list}")
        if addr is None:
            if '27' in i2c_list:
                self.LCD_ADDR = self.PCF8574_address
            elif '3f' in i2c_list:
                self.LCD_ADDR = self.PCF8574A_address
            else:
                raise IOError("I2C address 0x27 or 0x3f no found.")
        else:
            self.LCD_ADDR = addr
            if str(hex(addr)).strip('0x') not in i2c_list:
                raise IOError(f"I2C address {str(hex(addr))} or 0x3f no found.")    
        self.BLEN = bl
        try:
            self.send_command(0x33) # Must initialize to 8-line mode at first
            time.sleep(0.005)
            self.send_command(0x32) # Then initialize to 4-line mode
            time.sleep(0.005)
            self.send_command(0x28) # 2 Lines & 5*7 dots
            time.sleep(0.005)
            self.send_command(0x0C) # Enable display without cursor
            time.sleep(0.005)
            self.send_command(0x01) # Clear Screen
            self.buswrite_byte(self.LCD_ADDR, 0x08)
        except:
            return False
        else:
            return True

    def clear(self):
        self.send_command(0x01) # Clear Screen

    def openlight(self):  # Enable the backlight
        self.bus.write_byte(0x27,0x08)
        self.bus.close()

    def write(self,x, y, str):
        if x < 0:
            x = 0
        if x > 15:
            x = 15
        if y <0:
            y = 0
        if y > 1:
            y = 1
        # Move cursor
        addr = 0x80 + 0x40 * y + x
        self.send_command(addr)
        for chr in str:
            self.send_data(ord(chr))
    def display_num(self,x, y, num):
        addr = 0x80 + 0x40 * y + x
        self.send_command(addr)
        self.send_data(num)
        
def loop():
    count = 0
    while(True):
        lcd1602.clear()
        lcd1602.write(0, 0, '  Hello World!  ' )# display CPU temperature
        lcd1602.write(0, 1, '  Counter: ' + str(count) )   # display the time
        time.sleep(1)
        count += 1
def destroy():
    lcd1602.clear()
lcd1602 = CharLCD1602()  
if __name__ == '__main__':
    print ('Program is starting ... ')
    lcd1602.init_lcd(addr=None, bl=1)
    try:
        loop()
    except KeyboardInterrupt:
        destroy()

Traceback (most recent call last):
  File "/usr/lib/python3/dist-packages/gpiozero/pins/pi.py", line 411, in pin
    pin = self.pins[info]
          ~~~~~~~~~^^^^^^
KeyError: PinInfo(number=36, name='GPIO16', names=frozenset({'J8:36', 16, 'WPI27', 'GPIO16', 'BOARD36', 'BCM16', '16'}), pull='', row=18, col=2, interfaces=frozenset({'', 'uart', 'gpio', 'dpi', 'spi'}))

During handling of the above exception, another exception occurred:

Traceback (most recent call last):
  File "/home/cajote/Downloads/Desafio.py", line 181, in <module>
    loop()
    ~~~~^^
  File "/home/cajote/Downloads/Desafio.py", line 120, in loop
    tecla = keypad.getKey()
  File "/home/cajote/Downloads/Keypad.py", line 49, in getKey
    if(self.getKeys() and self.key[0].stateChanged and (self.key[0].kstate == self.key[0].PRESSED)):
       ~~~~~~~~~~~~^^
  File "/home/cajote/Downloads/Keypad.py", line 58, in getKeys
    self.scanKeys()
    ~~~~~~~~~~~~~^^
  File "/home/cajote/Downloads/Keypad.py", line 65, in scanKeys
    inputs = list(map(lambda pin: InputDevice(pin, pull_up=True), self.rowPins))
  File "/home/cajote/Downloads/Keypad.py", line 65, in <lambda>
    inputs = list(map(lambda pin: InputDevice(pin, pull_up=True), self.rowPins))
                                  ~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^
  File "/usr/lib/python3/dist-packages/gpiozero/devices.py", line 108, in __call__
    self = super().__call__(*args, **kwargs)
  File "/usr/lib/python3/dist-packages/gpiozero/input_devices.py", line 79, in __init__
    super().__init__(pin, pin_factory=pin_factory)
    ~~~~~~~~~~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/usr/lib/python3/dist-packages/gpiozero/devices.py", line 553, in __init__
    pin = self.pin_factory.pin(pin)
  File "/usr/lib/python3/dist-packages/gpiozero/pins/pi.py", line 413, in pin
    pin = self.pin_class(self, info)
  File "/usr/lib/python3/dist-packages/gpiozero/pins/lgpio.py", line 126, in __init__
    lgpio.gpio_claim_input(
    ~~~~~~~~~~~~~~~~~~~~~~^
        self.factory._handle, self._number, lgpio.SET_PULL_NONE)
        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/usr/lib/python3/dist-packages/lgpio.py", line 755, in gpio_claim_input
    return _u2i(_lgpio._gpio_claim_input(handle&0xffff, lFlags, gpio))
  File "/usr/lib/python3/dist-packages/lgpio.py", line 458, in _u2i
    raise error(error_text(v))
lgpio.error: 'GPIO busy'


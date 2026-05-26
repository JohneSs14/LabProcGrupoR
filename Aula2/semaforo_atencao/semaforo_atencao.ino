#ifndef RGB_BUILTIN
#define RGB_BUILTIN 8
#endif

#define DELAY_T 500  // ms por meio-ciclo (~1 s piscando)

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  neopixelWrite(RGB_BUILTIN, r, g, b);
}

void setup() {
  setColor(0, 0, 0);
}

void loop() {
  setColor(255, 180, 0);  // amarelo
  delay(DELAY_T);
  setColor(0, 0, 0);      // apagado
  delay(DELAY_T);
}

/*
  Blink — Missão 2 (LED interno da placa)
  Wokwi ESP32 DevKit: GPIO 2 | ESP32-C3 Super Mini: mude LED_BUILTIN para 8
*/

#define DELAY_MS 1000  // 1000 lento | 200 rápido | 50 atenção

#ifndef LED_BUILTIN
#define LED_BUILTIN 2   // LED interno no Wokwi (ESP32 DevKit)
#endif

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(DELAY_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(DELAY_MS);
}

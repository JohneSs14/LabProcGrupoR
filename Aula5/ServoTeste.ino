#include <Arduino.h>

const int SERVO_PIN = 5;
const int SERVO_FREQ = 50;
const int SERVO_RES = 14;

uint32_t anguloParaDuty(int angulo) {
  int pulsoUs = map(angulo, 0, 180, 500, 2500);
  return (uint32_t)pulsoUs * SERVO_FREQ * (1UL << SERVO_RES) / 1000000UL;
}

void moverServo(int angulo) {
  if (angulo < 0) angulo = 0;
  if (angulo > 180) angulo = 180;

  ledcWrite(SERVO_PIN, anguloParaDuty(angulo));
}

void setup() {
  ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_RES);
}

void loop() {
  moverServo(0);
  delay(1000);

  moverServo(90);
  delay(1000);

  moverServo(180);
  delay(1000);
}

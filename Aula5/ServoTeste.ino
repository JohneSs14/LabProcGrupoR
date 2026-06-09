#include <Arduino.h>

const int SERVO_PIN = 5;
const int SERVO_FREQ = 50;
const int SERVO_RES = 14; // ESP32-C3: LEDC aceita no maximo 14 bits (nao 16)

uint32_t anguloParaDuty(int angulo) {
  int pulsoUs = map(angulo, 0, 180, 500, 2500);
  return (uint32_t)pulsoUs * SERVO_FREQ * (1UL << SERVO_RES) / 1000000UL;
}

void moverServo(int angulo) {
  if (angulo < 0) angulo = 0;
  if (angulo > 180) angulo = 180;

  ledcWrite(SERVO_PIN, anguloParaDuty(angulo));

  Serial.print("Servo angulo=");
  Serial.println(angulo);
}

void setup() {
  Serial.begin(115200);

  if (!ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_RES)) {
    Serial.println("ERRO: ledcAttach falhou. Confira GPIO e resolucao.");
  } else {
    Serial.println("ServoTeste: 0 -> 90 -> 180");
  }
}

void loop() {
  moverServo(0);
  delay(1000);
  moverServo(90);
  delay(1000);
  moverServo(180);
  delay(1000);
}

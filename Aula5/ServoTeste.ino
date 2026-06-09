#include <Arduino.h>

const int SERVO_PIN = 5;
const int SERVO_FREQ = 50;
const int SERVO_RES = 16;

void moverServo(int angulo) {
  if (angulo < 0) angulo = 0;
  if (angulo > 180) angulo = 180;

  int duty = map(angulo, 0, 180, 1638, 8191);

  ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_RES);
  ledcWrite(SERVO_PIN, duty);

  Serial.print("Servo angulo=");
  Serial.println(angulo);
}

void setup() {
  Serial.begin(115200);
  Serial.println("ServoTeste: 0 -> 90 -> 180");
}

void loop() {
  moverServo(0);
  delay(1000);
  moverServo(90);
  delay(1000);
  moverServo(180);
  delay(1000);
}

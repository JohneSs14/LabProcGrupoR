#include <Arduino.h>

const int SOS_PIN = 3;

void setup() {
  Serial.begin(115200);
  pinMode(SOS_PIN, INPUT);
}

void loop() {

  if (digitalRead(SOS_PIN) == HIGH) {
    Serial.println("BOTAO PRESSIONADO");
    delay(200); // evita spam no terminal
  }

}

#include <Arduino.h>

const int SOS_PIN = 3;
const unsigned long DEBOUNCE_MS = 200;

int lastButtonState = LOW;
int buttonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned int contadorPressoes = 0;

void setup() {
  Serial.begin(115200);
  pinMode(SOS_PIN, INPUT);

  lastButtonState = digitalRead(SOS_PIN);
  buttonState = lastButtonState;

  Serial.println("Teste botao SOS com debounce (GPIO3, HIGH = pressionado)");
}

void loop() {

  int reading = digitalRead(SOS_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        contadorPressoes++;
        Serial.print("BOTAO PRESSIONADO | Total: ");
        Serial.println(contadorPressoes);
      } else {
        Serial.println("BOTAO SOLTO");
      }
    }
  }

  lastButtonState = reading;
}

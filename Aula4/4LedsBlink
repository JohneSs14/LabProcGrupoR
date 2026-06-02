const int LEDS[4] = {4, 5, 6, 7};

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], HIGH);
    if (i == 0) {
      digitalWrite(LEDS[3], LOW);
    } else {
      digitalWrite(LEDS[i-1], LOW);
    }
    delay(500);
  }
}

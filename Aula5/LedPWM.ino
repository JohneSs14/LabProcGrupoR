const int LED_PIN = 4;
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;

void setup() {
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);
}

void loop() {
  ledcWrite(LED_PIN, 0);
  delay(1000);

  ledcWrite(LED_PIN, 64);
  delay(1000);

  ledcWrite(LED_PIN, 128);
  delay(1000);

  ledcWrite(LED_PIN, 191);
  delay(1000);

  ledcWrite(LED_PIN, 255);
  delay(1000);
}

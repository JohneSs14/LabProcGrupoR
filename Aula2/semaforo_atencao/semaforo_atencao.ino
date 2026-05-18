#define PIN_YELLOW 4
#define DELAY_T    500   // ms por meio-ciclo (~1 s piscando)

void setup() {
  pinMode(PIN_YELLOW, OUTPUT);
  digitalWrite(PIN_YELLOW, LOW);
}

void loop() {
  digitalWrite(PIN_YELLOW, HIGH);
  delay(DELAY_T);
  digitalWrite(PIN_YELLOW, LOW);
  delay(DELAY_T);
}

/*
  Missão 4 — Semáforo de atenção (garagem): só AMARELO piscante.
  Estados: HIGH -> delay(T) -> LOW -> delay(T) -> repete.
  Verde e vermelho desligados (use só LED amarelo ou os 3 com G/R em LOW).

  Wokwi: GPIO 4 -> resistor -> LED amarelo; cátodo -> GND.
*/

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

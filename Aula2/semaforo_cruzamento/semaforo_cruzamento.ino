#define PIN_GREEN  16
#define PIN_YELLOW 4
#define PIN_RED    5

#define TEMPO_VERDE_MS    3000
#define TEMPO_VERMELHO_MS 4000
#define TEMPO_AMARELO_MS  1000

enum Estado { VERDE, VERMELHO, AMARELO };

Estado estadoAtual = VERDE;
unsigned long instanteEstado = 0;

void acende(Estado e) {
  digitalWrite(PIN_GREEN,  (e == VERDE)    ? HIGH : LOW);
  digitalWrite(PIN_YELLOW, (e == AMARELO)  ? HIGH : LOW);
  digitalWrite(PIN_RED,    (e == VERMELHO) ? HIGH : LOW);
}

void mudarEstado(Estado proximo) {
  estadoAtual = proximo;
  instanteEstado = millis();
  acende(proximo);
}

void setup() {
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  mudarEstado(VERDE);
}

void loop() {
  unsigned long decorrido = millis() - instanteEstado;

  switch (estadoAtual) {
    case VERDE:
      if (decorrido >= TEMPO_VERDE_MS) {
        mudarEstado(AMARELO);
      }
      break;
    case VERMELHO:
      if (decorrido >= TEMPO_VERMELHO_MS) {
        mudarEstado(VERDE);
      }
      break;
    case AMARELO:
      if (decorrido >= TEMPO_AMARELO_MS) {
        mudarEstado(VERMELHO);
      }
      break;
  }
}

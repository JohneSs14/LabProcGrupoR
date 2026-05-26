#ifndef RGB_BUILTIN
#define RGB_BUILTIN 8
#endif

#define TEMPO_VERDE_MS    3000
#define TEMPO_VERMELHO_MS 4000
#define TEMPO_AMARELO_MS  1000

enum Estado { VERDE, VERMELHO, AMARELO };

Estado estadoAtual = VERDE;
unsigned long instanteEstado = 0;

void acende(Estado e) {
  switch (e) {
    case VERDE:
      neopixelWrite(RGB_BUILTIN, 0, 255, 0);
      break;
    case AMARELO:
      neopixelWrite(RGB_BUILTIN, 255, 180, 0);
      break;
    case VERMELHO:
      neopixelWrite(RGB_BUILTIN, 255, 0, 0);
      break;
  }
}

void mudarEstado(Estado proximo) {
  estadoAtual = proximo;
  instanteEstado = millis();
  acende(proximo);
}

void setup() {
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

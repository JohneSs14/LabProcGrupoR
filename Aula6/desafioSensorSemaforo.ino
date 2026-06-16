#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

const int analogPin = 0;
const int builtin = 8;
const int SOS_PIN = 3;

const int LIMIAR_BAIXA_LUZ = 3000;

const unsigned long INTERVALO_PISCA_MS = 2000;
const unsigned long DEBOUNCE_MS = 50;
const unsigned long TEMPO_ALERTA_SOS = 3000;

const unsigned long TEMPO_VERDE_MS = 3000;
const unsigned long TEMPO_VERMELHO_MS = 4000;
const unsigned long TEMPO_AMARELO_MS = 1000;

enum Estado { VERDE, VERMELHO, AMARELO };

Estado estadoAtual = VERDE;

bool baixaLuminosidade = false;
bool ledAmareloLigado = false;

unsigned long instantePisca = 0;
unsigned long instanteEstado = 0;

bool estadoEstavelSOS = LOW;
bool ultimaLeituraSOS = LOW;
unsigned long ultimoTempoMudancaSOS = 0;

bool alertaSOS = false;
unsigned long inicioAlertaSOS = 0;

void apagaLed() {
  neopixelWrite(builtin, 0, 0, 0);
}

void acendeAmarelo() {
  neopixelWrite(builtin, 255, 180, 0);
}

void acendeVermelho() {
  neopixelWrite(builtin, 255, 0, 0);
}

void acendeVerde() {
  neopixelWrite(builtin, 0, 255, 0);
}

void acendeSemaforo(Estado e) {
  switch (e) {
    case VERDE:
      acendeVerde();
      break;

    case AMARELO:
      acendeAmarelo();
      break;

    case VERMELHO:
      acendeVermelho();
      break;
  }
}

void mudarEstado(Estado proximo) {
  estadoAtual = proximo;
  instanteEstado = millis();
  acendeSemaforo(proximo);
}

void setup() {

  Serial.begin(115200);

  pinMode(SOS_PIN, INPUT);

  instantePisca = millis();

  mudarEstado(VERDE);

  WiFi.mode(WIFI_AP);

  bool ok = WiFi.softAP(ssid, password);

  if (ok) {
    Serial.println("Access Point criado!");
    Serial.print("IP do AP: ");
    Serial.println(WiFi.softAPIP());
  }

  server.begin();
}

void loop() {

  bool leituraSOS = digitalRead(SOS_PIN);

  if (leituraSOS != ultimaLeituraSOS) {
    ultimoTempoMudancaSOS = millis();
    ultimaLeituraSOS = leituraSOS;
  }

  if ((millis() - ultimoTempoMudancaSOS) > DEBOUNCE_MS) {

    if (leituraSOS != estadoEstavelSOS) {

      estadoEstavelSOS = leituraSOS;

      if (estadoEstavelSOS == HIGH) {

        Serial.println("BOTAO PRESSIONADO");

        alertaSOS = true;
        inicioAlertaSOS = millis();

        acendeVermelho();

      } else {

        Serial.println("BOTAO NAO PRESSIONADO");
      }
    }
  }

  int sensorValue = analogRead(analogPin);

  baixaLuminosidade = sensorValue > LIMIAR_BAIXA_LUZ;

  unsigned long agora = millis();

  if (alertaSOS) {

    acendeVermelho();

    if (agora - inicioAlertaSOS >= TEMPO_ALERTA_SOS) {

      alertaSOS = false;

      instanteEstado += TEMPO_ALERTA_SOS;

      if (!baixaLuminosidade) {
        acendeSemaforo(estadoAtual);
      }
    }

  } else {

    if (baixaLuminosidade) {

      if (agora - instantePisca >= INTERVALO_PISCA_MS) {

        instantePisca = agora;
        ledAmareloLigado = !ledAmareloLigado;

        if (ledAmareloLigado) {
          acendeAmarelo();
        } else {
          apagaLed();
        }
      }

    } else {

      unsigned long decorrido = agora - instanteEstado;

      switch (estadoAtual) {

        case VERDE:

          if (decorrido >= TEMPO_VERDE_MS) {
            mudarEstado(AMARELO);
          }

          break;

        case AMARELO:

          if (decorrido >= TEMPO_AMARELO_MS) {
            mudarEstado(VERMELHO);
          }

          break;

        case VERMELHO:

          if (decorrido >= TEMPO_VERMELHO_MS) {
            mudarEstado(VERDE);
          }

          break;
      }
    }
  }

  WiFiClient client = server.accept();

  if (client) {

    String currentLine = "";

    while (client.connected()) {

      if (client.available()) {

        char c = client.read();

        if (c == '\n') {

          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE html>");
            client.println("<html>");

            client.println("<head>");
            client.println("<meta charset='UTF-8'>");
            client.println("<title>Monitoramento</title>");
            client.println("<script>");
            client.println("setTimeout(function(){ location.reload(); }, 1000);");
            client.println("</script>");
            client.println("</head>");

            client.println("<body style='font-family:Arial; text-align:center; margin-top:50px;'>");

            client.println("<h1>Monitoramento do Sistema</h1>");

            client.print("<h2>Valor Atual do Sensor: ");
            client.print(sensorValue);
            client.println("</h2>");

            client.print("<p>Estado da Luminosidade: ");
            client.print(baixaLuminosidade ? "Baixa luminosidade" : "Luminosidade alta");
            client.println("</p>");

            client.println("<hr>");
            client.println("<h2>Botao SOS</h2>");

            if (estadoEstavelSOS == HIGH) {
              client.println("<h3 style='color:red;'>BOTAO PRESSIONADO</h3>");
            } else {
              client.println("<h3 style='color:green;'>BOTAO NAO PRESSIONADO</h3>");
            }

            client.println("<hr>");
            client.println("<h2>Estado do LED</h2>");

            if (alertaSOS) {

              client.println("<h3 style='color:red;'>ALERTA SOS ATIVO</h3>");

            } else if (baixaLuminosidade) {

              client.println("<h3 style='color:orange;'>MODO BAIXA LUMINOSIDADE</h3>");

            } else {

              client.print("<h3>");

              switch (estadoAtual) {

                case VERDE:
                  client.print("SEMAFORO VERDE");
                  break;

                case AMARELO:
                  client.print("SEMAFORO AMARELO");
                  break;

                case VERMELHO:
                  client.print("SEMAFORO VERMELHO");
                  break;
              }

              client.println("</h3>");
            }

            client.println("<p>Atualizacao automatica a cada 1 segundo</p>");

            client.println("</body>");
            client.println("</html>");

            client.println();

            break;

          } else {

            currentLine = "";
          }

        } else if (c != '\r') {

          currentLine += c;
        }
      }
    }

    client.stop();
  }

  delay(10);
}

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
const unsigned long DEBOUNCE_MS = 200;
const unsigned long SOS_DURACAO_MS = 3000;

bool baixaLuminosidade = false;
bool ledAmareloLigado = false;
bool sosAtivo = false;

unsigned long instantePisca = 0;
unsigned long instanteSosFim = 0;
unsigned long ultimoDebounceMs = 0;

volatile bool interrupcaoSos = false;

void IRAM_ATTR onSos() {
  interrupcaoSos = true;
}

void apagaLed() {
  neopixelWrite(builtin, 0, 0, 0);
}

void acendeAmarelo() {
  neopixelWrite(builtin, 255, 180, 0);
}

void acendeVermelho() {
  neopixelWrite(builtin, 255, 0, 0);
}

void trataSos() {

  if (!interrupcaoSos) {
    return;
  }

  unsigned long agora = millis();

  if (agora - ultimoDebounceMs < DEBOUNCE_MS) {
    return;
  }

  interrupcaoSos = false;
  ultimoDebounceMs = agora;

  sosAtivo = true;
  instanteSosFim = agora + SOS_DURACAO_MS;

  Serial.println("SOS: interrupcao detectada");
}

void atualizaLed() {

  unsigned long agora = millis();

  if (sosAtivo && agora < instanteSosFim) {
    acendeVermelho();
    return;
  }

  sosAtivo = false;

  if (!baixaLuminosidade) {
    apagaLed();
    ledAmareloLigado = false;
    return;
  }

  if (agora - instantePisca >= INTERVALO_PISCA_MS) {
    instantePisca = agora;
    ledAmareloLigado = !ledAmareloLigado;

    if (ledAmareloLigado) {
      acendeAmarelo();
    } else {
      apagaLed();
    }
  }
}

void setup() {

  Serial.begin(115200);

  instantePisca = millis();
  apagaLed();

  pinMode(SOS_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SOS_PIN), onSos, FALLING);

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

  int sensorValue = analogRead(analogPin);

  baixaLuminosidade = sensorValue > LIMIAR_BAIXA_LUZ;

  trataSos();

  Serial.print(sensorValue);
  Serial.print(" | Baixa luz=");
  Serial.print(baixaLuminosidade ? "sim" : "nao");
  Serial.print(" | SOS=");
  Serial.println(sosAtivo ? "sim" : "nao");

  atualizaLed();

  WiFiClient client = server.accept();

  if (client) {

    String currentLine = "";

    while (client.connected()) {

      trataSos();
      atualizaLed();

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
            client.println("<title>Monitoramento Analogico</title>");

            client.println("<script>");
            client.println("setTimeout(function(){ location.reload(); }, 1000);");
            client.println("</script>");

            client.println("</head>");

            client.println("<body style='font-family: Arial; text-align: center; margin-top: 50px;'>");

            client.println("<h1>Monitoramento de Sensor Analogico</h1>");

            client.print("<h2>Valor Atual: ");
            client.print(sensorValue);
            client.println("</h2>");

            client.print("<p>Estado: ");
            client.print(baixaLuminosidade ? "Baixa luminosidade" : "Luminosidade normal");
            client.println("</p>");

            client.print("<p>SOS: ");
            client.print(sosAtivo ? "Ativo" : "Inativo");
            client.println("</p>");

            client.println("<p>Atualização automática a cada 1 segundo</p>");

            client.println("</body>");
            client.println("</html>");

            client.println();

            break;
          }

          else {
            currentLine = "";
          }
        }

        else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    client.stop();
  }

  delay(100);
}

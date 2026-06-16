#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

const int analogPin = 0;

#ifndef RGB_BUILTIN
#define RGB_BUILTIN 8
#endif

const int LIMIAR_BAIXA_LUZ = 1500;
const unsigned long INTERVALO_PISCA_MS = 2000;

bool baixaLuminosidade = false;
bool ledAmareloLigado = false;
unsigned long instantePisca = 0;

void apagaLed() {
  neopixelWrite(RGB_BUILTIN, 0, 0, 0);
}

void acendeAmarelo() {
  neopixelWrite(RGB_BUILTIN, 255, 180, 0);
}

void setup() {

  Serial.begin(115200);

  instantePisca = millis();
  apagaLed();

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

  baixaLuminosidade = sensorValue < LIMIAR_BAIXA_LUZ;

  Serial.print(sensorValue);
  Serial.print(" | Baixa luz=");
  Serial.println(baixaLuminosidade ? "sim" : "nao");

  if (!baixaLuminosidade) {
    apagaLed();
    ledAmareloLigado = false;
  } else {
    unsigned long agora = millis();

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

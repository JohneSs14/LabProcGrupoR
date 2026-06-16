#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

const int analogPin = 0;

void setup() {

  Serial.begin(115200);

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

  Serial.println(sensorValue);

  WiFiClient client = server.accept();

  if (client) {

    String currentLine = "";

    while (client.connected()) {

      if (client.available()) {

        char c = client.read();

        if (c == '\n') {

          if (currentLine.length() == 0) {

            // Cabeçalho HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Página HTML
            client.println("<!DOCTYPE html>");
            client.println("<html>");

            client.println("<head>");
            client.println("<meta charset='UTF-8'>");
            client.println("<title>Monitoramento Analogico</title>");

            // Atualiza a página a cada 500 ms
            client.println("<script>");
            client.println("setTimeout(function(){ location.reload(); }, 1000);");
            client.println("</script>");

            client.println("</head>");

            client.println("<body style='font-family: Arial; text-align: center; margin-top: 50px;'>");

            client.println("<h1>Monitoramento de Sensor Analogico</h1>");

            client.print("<h2>Valor Atual: ");
            client.print(sensorValue);
            client.println("</h2>");

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

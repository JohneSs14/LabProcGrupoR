#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

const int LED_PIN = 4;
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;

int dutyCycle = 0;

void setup() {

  Serial.begin(115200);

  ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);
  ledcWrite(LED_PIN, dutyCycle);

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

  WiFiClient client = server.accept();

  if (client) {

    String currentLine = "";
    String request = "";

    while (client.connected()) {

      if (client.available()) {

        char c = client.read();

        request += c;

        if (c == '\n') {

          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            if (request.indexOf("GET /setPWM?") >= 0) {

              int start = request.indexOf("value=") + 6;
              int end = request.indexOf(" ", start);

              String valueStr = request.substring(start, end);

              dutyCycle = valueStr.toInt();

              if (dutyCycle < 0) dutyCycle = 0;
              if (dutyCycle > 255) dutyCycle = 255;

              ledcWrite(LED_PIN, dutyCycle);
            }

            client.println("<!DOCTYPE html>");
            client.println("<html>");

            client.println("<head>");
            client.println("<title>Controle PWM</title>");
            client.println("</head>");

            client.println("<body>");

            client.println("<h1>Controle de Intensidade do LED</h1>");

            client.println("<form action=\"/setPWM\" method=\"GET\">");

            client.print("<p>Duty Cycle Atual: ");
            client.print(dutyCycle);
            client.println("</p>");

            client.print("<input type=\"range\" ");
            client.print("name=\"value\" ");
            client.print("min=\"0\" ");
            client.print("max=\"255\" ");
            client.print("value=\"");
            client.print(dutyCycle);
            client.println("\">");

            client.println("<br><br>");

            client.println("<input type=\"submit\" value=\"Atualizar\">");

            client.println("</form>");

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
}

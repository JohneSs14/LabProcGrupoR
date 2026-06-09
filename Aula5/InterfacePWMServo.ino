#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

const int SERVO_PIN = 5;
const int SERVO_FREQ = 50;
const int SERVO_RES = 14;

int anguloAtual = 90;

uint32_t anguloParaDuty(int angulo) {
  int pulsoUs = map(angulo, 0, 180, 500, 2500);
  return (uint32_t)pulsoUs * SERVO_FREQ * (1UL << SERVO_RES) / 1000000UL;
}

void moverServo(int angulo) {

  if (angulo < 0) angulo = 0;
  if (angulo > 180) angulo = 180;

  ledcWrite(SERVO_PIN, anguloParaDuty(angulo));
}

void setup() {

  ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_RES);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.begin();

  moverServo(anguloAtual);
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

            if (request.indexOf("GET /setServo?") >= 0) {

              int start = request.indexOf("value=") + 6;
              int end = request.indexOf(" ", start);

              String valueStr = request.substring(start, end);

              anguloAtual = valueStr.toInt();

              if (anguloAtual < 0) anguloAtual = 0;
              if (anguloAtual > 180) anguloAtual = 180;

              moverServo(anguloAtual);
            }

            client.println("<!DOCTYPE html>");
            client.println("<html>");

            client.println("<head>");
            client.println("<title>Controle de Servomotor</title>");
            client.println("</head>");

            client.println("<body>");

            client.println("<h1>Controle de Posicao do Servomotor</h1>");

            client.println("<form action=\"/setServo\" method=\"GET\">");

            client.print("<p>Angulo Atual: ");
            client.print(anguloAtual);
            client.println(" graus</p>");

            client.print("<input type=\"range\" ");
            client.print("name=\"value\" ");
            client.print("min=\"0\" ");
            client.print("max=\"180\" ");
            client.print("value=\"");
            client.print(anguloAtual);
            client.println("\">");

            client.println("<br><br>");

            client.println("<input type=\"submit\" value=\"Mover Servo\">");

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

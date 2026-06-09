#include <Arduino.h>
#include <WiFi.h>

// Etapa D: controle do servomotor pela interface web (independente do LED/PWM).

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

const int SERVO_PIN = 5;
const int SERVO_FREQ = 50;
const int SERVO_RES = 16;

int anguloAtual = 90;

void moverServo(int angulo) {
  if (angulo < 0) angulo = 0;
  if (angulo > 180) angulo = 180;

  anguloAtual = angulo;

  int duty = map(angulo, 0, 180, 1638, 8191);

  ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_RES);
  ledcWrite(SERVO_PIN, duty);

  Serial.print("Servo angulo=");
  Serial.println(anguloAtual);
}

void setup() {

  Serial.begin(115200);

  moverServo(90);

  WiFi.mode(WIFI_AP);
  bool ok = WiFi.softAP(ssid, password);

  if (ok) {
    Serial.println("Access Point criado!");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("IP do AP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Falha ao criar o Access Point.");
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

            if (request.indexOf("GET /servo?") >= 0) {

              int startAng = request.indexOf("ang=") + 4;
              int endAng = request.indexOf(" ", startAng);
              int angulo = request.substring(startAng, endAng).toInt();

              moverServo(angulo);

              client.println("<!DOCTYPE html>");
              client.println("<html>");
              client.println("<head>");
              client.println("<title>Servo</title>");
              client.println("</head>");
              client.println("<body>");

              client.println("<h1>Servo posicionado</h1>");

              client.print("<p>Angulo: ");
              client.print(anguloAtual);
              client.println(" graus</p>");

              client.println("<br>");
              client.println("<a href=\"/\">Voltar</a>");

              client.println("</body>");
              client.println("</html>");
            }

            else {

              client.println("<!DOCTYPE html>");
              client.println("<html>");
              client.println("<head>");
              client.println("<title>ESP32 Servo</title>");
              client.println("</head>");
              client.println("<body>");

              client.println("<h1>Controle Servomotor - GPIO 5</h1>");

              client.println("<form action=\"/servo\" method=\"GET\">");

              client.println("<label for=\"ang\">Angulo (0-180):</label>");
              client.print("<input type=\"number\" id=\"ang\" name=\"ang\" min=\"0\" max=\"180\" value=\"");
              client.print(anguloAtual);
              client.println("\" required>");

              client.println("<br><br>");

              client.println("<input type=\"submit\" value=\"Mover servo\">");

              client.println("</form>");

              client.println("</body>");
              client.println("</html>");
            }

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

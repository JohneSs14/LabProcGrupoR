#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

// ==================== LED ====================

const int LED_PIN = 4;
const int PWM_FREQ_LED = 5000;
const int PWM_RES_LED = 8;

int dutyCycle = 0;

// ==================== SERVO ====================

const int SERVO_PIN = 5;
const int SERVO_FREQ = 50;
const int SERVO_RES = 14;

int anguloAtual = 90;

uint32_t anguloParaDuty(int angulo) {
  int pulsoUs = map(angulo, 0, 180, 500, 2500);

  return (uint32_t)pulsoUs *
         SERVO_FREQ *
         (1UL << SERVO_RES) /
         1000000UL;
}

void moverServo(int angulo) {

  if (angulo < 0) angulo = 0;
  if (angulo > 180) angulo = 180;

  ledcWrite(SERVO_PIN, anguloParaDuty(angulo));
}

// ==================== SETUP ====================

void setup() {

  Serial.begin(115200);

  // LED PWM
  ledcAttach(LED_PIN, PWM_FREQ_LED, PWM_RES_LED);
  ledcWrite(LED_PIN, dutyCycle);

  // Servo PWM
  ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_RES);
  moverServo(anguloAtual);

  WiFi.mode(WIFI_AP);

  bool ok = WiFi.softAP(ssid, password);

  if (ok) {
    Serial.println("Access Point criado!");
    Serial.print("IP do AP: ");
    Serial.println(WiFi.softAPIP());
  }

  server.begin();
}

// ==================== LOOP ====================

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

            // ====================
            // Atualiza LED e Servo
            // ====================

            if (request.indexOf("GET /update?") >= 0) {

              // LED

              int startLed = request.indexOf("led=") + 4;
              int endLed = request.indexOf("&", startLed);

              if (startLed >= 4 && endLed > startLed) {

                String ledStr =
                    request.substring(startLed, endLed);

                dutyCycle = ledStr.toInt();

                if (dutyCycle < 0) dutyCycle = 0;
                if (dutyCycle > 255) dutyCycle = 255;

                ledcWrite(LED_PIN, dutyCycle);
              }

              // Servo

              int startServo =
                  request.indexOf("servo=") + 6;

              int endServo =
                  request.indexOf(" ", startServo);

              if (startServo >= 6 &&
                  endServo > startServo) {

                String servoStr =
                    request.substring(startServo, endServo);

                anguloAtual = servoStr.toInt();

                if (anguloAtual < 0) anguloAtual = 0;
                if (anguloAtual > 180) anguloAtual = 180;

                moverServo(anguloAtual);
              }
            }

            // ====================
            // Cabeçalho HTTP
            // ====================

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // ====================
            // Página HTML
            // ====================

            client.println("<!DOCTYPE html>");
            client.println("<html>");

            client.println("<head>");
            client.println("<title>Controle Integrado</title>");
            client.println("</head>");

            client.println("<body>");

            client.println("<h1>Controle de LED e Servomotor</h1>");

            client.println("<form action=\"/update\" method=\"GET\">");

            // LED

            client.println("<h2>LED PWM</h2>");

            client.print("<p>Duty Cycle Atual: ");
            client.print(dutyCycle);
            client.println("</p>");

            client.print("<input type=\"range\" ");
            client.print("name=\"led\" ");
            client.print("min=\"0\" ");
            client.print("max=\"255\" ");
            client.print("value=\"");
            client.print(dutyCycle);
            client.println("\">");

            client.println("<br><br>");

            // Servo

            client.println("<h2>Servomotor</h2>");

            client.print("<p>Angulo Atual: ");
            client.print(anguloAtual);
            client.println(" graus</p>");

            client.print("<input type=\"range\" ");
            client.print("name=\"servo\" ");
            client.print("min=\"0\" ");
            client.print("max=\"180\" ");
            client.print("value=\"");
            client.print(anguloAtual);
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

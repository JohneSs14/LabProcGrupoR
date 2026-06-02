#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

const int LEDS[4] = {4, 5, 6, 7};

int binToSigned4bit(String bin) {
  int value = strtol(bin.c_str(), NULL, 2);

  if (value & 0b1000) {
    value = value - 16;
  }

  return value;
}

String signedToBinary4bit(int value) {

  value = value & 0x0F;

  String result = "";

  for (int i = 3; i >= 0; i--) {
    result += ((value >> i) & 1) ? '1' : '0';
  }

  return result;
}

void showResultOnLEDs(int value) {

  value = value & 0x0F;

  for (int i = 0; i < 4; i++) {

    int bitValue = (value >> i) & 1;

    digitalWrite(LEDS[i], bitValue);
  }
}

void setup() {

  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(LEDS[i], OUTPUT);
  }

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

            if (request.indexOf("GET /calculate?") >= 0) {

              int resultado = 0;

              int startA = request.indexOf("opA=") + 4;
              int endA = request.indexOf("&", startA);

              String opAStr = request.substring(startA, endA);
              int opA = binToSigned4bit(opAStr);

              int startB = request.indexOf("opB=") + 4;
              int endB = request.indexOf("&", startB);

              String opBStr = request.substring(startB, endB);
              int opB = binToSigned4bit(opBStr);

              int startOp = request.indexOf("operation=") + 10;
              int endOp = request.indexOf(" ", startOp);

              String op = request.substring(startOp, endOp);

              if (op == "mul") {

                resultado = (strtol(opAStr.c_str(), NULL, 2) & 0x0F) * (strtol(opBStr.c_str(), NULL, 2) & 0x0F);

                bool overflow = false;

                if (resultado > 15) {
                  overflow = true;
                }

                showResultOnLEDs(resultado);

                String resultadoBin = signedToBinary4bit(resultado);

                client.println("<!DOCTYPE html>");
                client.println("<html>");

                client.println("<head>");
                client.println("<title>Resultado</title>");
                client.println("</head>");

                client.println("<body>");

                client.println("<h1>Resultado da ALU</h1>");

                client.print("<p>Resultado decimal: ");
                client.print(resultado);
                client.println("</p>");

                client.print("<p>Resultado binario (4 bits nos LEDs): ");
                client.print(resultadoBin);
                client.println("</p>");

                if (overflow) {
                  client.println("<p><b>OVERFLOW: produto maior que 15.</b></p>");
                }

                client.println("</body>");
                client.println("</html>");
              }

              else {

                resultado = (op == "sum") ? (opA + opB) : (opA - opB);

                bool overflow = false;

                if (resultado > 7 || resultado < -8) {
                  overflow = true;
                }

                if (overflow) {

                  client.println("<!DOCTYPE html>");
                  client.println("<html>");

                  client.println("<head>");
                  client.println("<title>Overflow</title>");
                  client.println("</head>");

                  client.println("<body>");

                  client.println("<h1>OVERFLOW DETECTADO</h1>");
                  client.println("<p>O resultado nao cabe em 4 bits signed.</p>");

                  client.println("</body>");
                  client.println("</html>");
                }

                else {

                  showResultOnLEDs(resultado);

                  String resultadoBin = signedToBinary4bit(resultado);

                  client.println("<!DOCTYPE html>");
                  client.println("<html>");

                  client.println("<head>");
                  client.println("<title>Resultado</title>");
                  client.println("</head>");

                  client.println("<body>");

                  client.println("<h1>Resultado da ALU</h1>");

                  client.print("<p>Resultado decimal: ");
                  client.print(resultado);
                  client.println("</p>");

                  client.print("<p>Resultado binario: ");
                  client.print(resultadoBin);
                  client.println("</p>");

                  client.println("</body>");
                  client.println("</html>");
                }
              }
            }

            else {

              client.println("<!DOCTYPE html>");
              client.println("<html>");

              client.println("<head>");
              client.println("<title>ESP32 ALU Interface</title>");
              client.println("</head>");

              client.println("<body>");

              client.println("<form action=\"/calculate\" method=\"GET\">");

              client.println("<label for=\"opA\">Operand A (4-bit):</label>");
              client.println("<input type=\"text\" id=\"opA\" name=\"opA\" pattern=\"[01]{4}\" maxlength=\"4\" required>");

              client.println("<br><br>");

              client.println("<label for=\"opB\">Operand B (4-bit):</label>");
              client.println("<input type=\"text\" id=\"opB\" name=\"opB\" pattern=\"[01]{4}\" maxlength=\"4\" required>");

              client.println("<br><br>");

              client.println("<label for=\"operation\">Operation:</label>");

              client.println("<select id=\"operation\" name=\"operation\" required>");
              client.println("<option value=\"sum\">Sum (Add)</option>");
              client.println("<option value=\"sub\">Sub (Subtract)</option>");
              client.println("<option value=\"mul\">Mul (Multiply)</option>");
              client.println("</select>");

              client.println("<br><br>");

              client.println("<input type=\"submit\" value=\"Send to ESP32\">");

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

#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

unsigned long long binToDecimal(String bin) {
  return strtoull(bin.c_str(), NULL, 2);
}

String toBinary(unsigned long long value) {

  if (value == 0) {
    return "0";
  }

  String result = "";

  while (value > 0) {
    result = String(value % 2) + result;
    value /= 2;
  }

  return result;
}

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_AP);

  bool ok = WiFi.softAP(ssid, password);

  if (ok) {
    Serial.println("Access Point criado!");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("IP do AP: ");
    Serial.println(WiFi.softAPIP());
  }
  else {
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

              unsigned long long resultado = 0;
              unsigned long tempoExecucao = 0;

              int startA = request.indexOf("opA=") + 4;
              int endA = request.indexOf("&", startA);

              String opAStr = request.substring(startA, endA);
              unsigned long long opA = binToDecimal(opAStr);

              int startB = request.indexOf("opB=") + 4;
              int endB = request.indexOf("&", startB);

              String opBStr = request.substring(startB, endB);
              unsigned long long opB = binToDecimal(opBStr);

              int startOp = request.indexOf("operation=") + 10;
              int endOp = request.indexOf(" ", startOp);

              String op = request.substring(startOp, endOp);

              if (op == "sum") {

                resultado = opA + opB;
              }

              else if (op == "sub") {

                if (opA >= opB) {
                  resultado = opA - opB;
                } else {
                  resultado = 0;
                }
              }

              else if (op == "mul") {

                unsigned long tInicio = micros();

                resultado = opA * opB;

                tempoExecucao = micros() - tInicio;

                Serial.print("Tempo mul: ");
                Serial.print(tempoExecucao);
                Serial.println(" us");
              }

              else if (op == "fat") {

                unsigned long tInicio = micros();

                resultado = 1;

                for (unsigned long long i = 2; i <= opA; i++) {
                  resultado *= i;
                }

                tempoExecucao = micros() - tInicio;

                Serial.print("Tempo fat: ");
                Serial.print(tempoExecucao);
                Serial.println(" us");
              }

              client.println("<!DOCTYPE html>");
              client.println("<html>");
              client.println("<head>");
              client.println("<meta charset=\"UTF-8\">");
              client.println("<title>Resultado</title>");
              client.println("</head>");
              client.println("<body>");

              client.println("<h1>Resultado da ALU</h1>");

              client.print("<p><b>Operacao:</b> ");
              client.print(op);
              client.println("</p>");

              client.print("<p><b>Resultado decimal:</b> ");
              client.print(resultado);
              client.println("</p>");

              client.print("<p><b>Resultado binario:</b> ");
              client.print(toBinary(resultado));
              client.println("</p>");

              if (op == "mul" || op == "fat") {

                client.print("<p><b>Tempo de execucao:</b> ");
                client.print(tempoExecucao);
                client.println(" us</p>");
              }

              client.println("<br>");
              client.println("<a href=\"/\">Voltar</a>");

              client.println("</body>");
              client.println("</html>");
            }

            else {

              client.println("<!DOCTYPE html>");
              client.println("<html>");
              client.println("<head>");
              client.println("<meta charset=\"UTF-8\">");
              client.println("<title>ESP32 ALU</title>");
              client.println("</head>");
              client.println("<body>");

              client.println("<h1>ESP32 ALU</h1>");

              client.println("<form action=\"/calculate\" method=\"GET\">");

              client.println("<label for=\"opA\">Operando A (binario):</label><br>");
              client.println("<input type=\"text\" id=\"opA\" name=\"opA\" pattern=\"[01]+\" required>");

              client.println("<br><br>");

              client.println("<label for=\"opB\">Operando B (binario):</label><br>");
              client.println("<input type=\"text\" id=\"opB\" name=\"opB\" pattern=\"[01]+\" required>");

              client.println("<br><br>");

              client.println("<label for=\"operation\">Operacao:</label><br>");

              client.println("<select id=\"operation\" name=\"operation\">");
              client.println("<option value=\"sum\">sum</option>");
              client.println("<option value=\"sub\">sub</option>");
              client.println("<option value=\"mul\">mul</option>");
              client.println("<option value=\"fat\">fat</option>");
              client.println("</select>");

              client.println("<br><br>");

              client.println("<input type=\"submit\" value=\"Executar\">");

              client.println("</form>");

              client.println("<p><b>fat</b> utiliza apenas o Operando A.</p>");

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

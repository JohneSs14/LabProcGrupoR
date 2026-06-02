#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

int binToSigned4bit(String bin) {
  int value = strtol(bin.c_str(), NULL, 2);

  if (value & 0b1000) {
    value = value - 16;
  }

  return value;
}

String toBinary(unsigned long long value) {
  if (value == 0) {
    return "0";
  }

  String result = "";

  bool started = false;

  for (int i = 63; i >= 0; i--) {
    bool bit = (value >> i) & 1;

    if (bit) {
      started = true;
    }

    if (started) {
      result += bit ? '1' : '0';
    }
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

              unsigned long long resultado = 0;
              unsigned long tempoExecucao = 0;
              bool divisaoPorZero = false;

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

              if (op == "sum") {

                resultado = opA + opB;
              }

              else if (op == "sub") {

                resultado = opA - opB;
              }

              else if (op == "mul") {

                unsigned long tInicio = micros();

                resultado =
                  (strtol(opAStr.c_str(), NULL, 2) & 0x0F) *
                  (strtol(opBStr.c_str(), NULL, 2) & 0x0F);

                tempoExecucao = micros() - tInicio;

                Serial.print("Tempo mul: ");
                Serial.print(tempoExecucao);
                Serial.println(" us");
              }

              else if (op == "fat") {

                int n = strtol(opAStr.c_str(), NULL, 2) & 0x0F;

                unsigned long tInicio = micros();

                resultado = 1;

                for (int i = 2; i <= n; i++) {
                  resultado *= i;
                }

                tempoExecucao = micros() - tInicio;

                Serial.print("Tempo fat: ");
                Serial.print(tempoExecucao);
                Serial.println(" us");
              }

              else if (op == "div") {

                int ua = strtol(opAStr.c_str(), NULL, 2) & 0x0F;
                int ub = strtol(opBStr.c_str(), NULL, 2) & 0x0F;

                unsigned long tInicio = micros();

                if (ub == 0) {
                  divisaoPorZero = true;
                  resultado = 0;
                } else {
                  resultado = ua / ub;
                }

                tempoExecucao = micros() - tInicio;

                Serial.print("Tempo div: ");
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

              if (divisaoPorZero) {
                client.println("<p><b>Erro: divisao por zero.</b></p>");
              } else {
                client.print("<p><b>Resultado decimal:</b> ");
                client.print(resultado);
                client.println("</p>");

                client.print("<p><b>Resultado binario:</b> ");
                client.print(toBinary(resultado));
                client.println("</p>");
              }

              if (op == "mul" || op == "fat" || op == "div") {

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

              client.println("<h1>ESP32 ALU - Desafio Divisao</h1>");

              client.println("<form action=\"/calculate\" method=\"GET\">");

              client.println("<label for=\"opA\">Operando A (4 bits):</label><br>");
              client.println("<input type=\"text\" id=\"opA\" name=\"opA\" pattern=\"[01]{4}\" maxlength=\"4\" required>");

              client.println("<br><br>");

              client.println("<label for=\"opB\">Operando B (4 bits):</label><br>");
              client.println("<input type=\"text\" id=\"opB\" name=\"opB\" pattern=\"[01]{4}\" maxlength=\"4\" required>");

              client.println("<br><br>");

              client.println("<label for=\"operation\">Operacao:</label><br>");

              client.println("<select id=\"operation\" name=\"operation\">");
              client.println("<option value=\"sum\">sum</option>");
              client.println("<option value=\"sub\">sub</option>");
              client.println("<option value=\"mul\">mul</option>");
              client.println("<option value=\"fat\">fat</option>");
              client.println("<option value=\"div\">div</option>");
              client.println("</select>");

              client.println("<br><br>");

              client.println("<input type=\"submit\" value=\"Executar\">");

              client.println("</form>");

              client.println("<p><b>fat</b> utiliza apenas o Operando A.</p>");
              client.println("<p><b>div</b> utiliza A e B (unsigned). Sem overflow.</p>");

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

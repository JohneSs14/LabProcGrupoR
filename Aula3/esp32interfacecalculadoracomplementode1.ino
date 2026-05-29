#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "RedeSuperManeira";
const char *password = "12345678";

WiFiServer server(80);

const int LEDS[4] = {4, 5, 6, 7};

String getDecimalFrom1sComp(uint8_t binValue) {
  if (binValue == 0b1111) return "-0";
  
  if (binValue & 0b1000) {
    int val = -((~binValue) & 0b0111);
    return String(val);
  }
  return String(binValue);
}

String binaryToString4bit(uint8_t value) {
  String result = "";
  for (int i = 3; i >= 0; i--) {
    result += ((value >> i) & 1) ? '1' : '0';
  }
  return result;
}

void showResultOnLEDs(uint8_t value) {
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
              
              int startA = request.indexOf("opA=") + 4;
              int endA = request.indexOf("&", startA);
              uint8_t opA = strtol(request.substring(startA, endA).c_str(), NULL, 2);

              int startB = request.indexOf("opB=") + 4;
              int endB = request.indexOf("&", startB);
              uint8_t opB = strtol(request.substring(startB, endB).c_str(), NULL, 2);

              int startOp = request.indexOf("operation=") + 10;
              int endOp = request.indexOf(" ", startOp);
              String op = request.substring(startOp, endOp);

              uint8_t opB_internal = opB;
              
              if (op == "sub") {
                opB_internal = (~opB) & 0x0F; 
              }

              uint16_t resultado = opA + opB_internal;

              if (resultado > 15) {
                resultado = (resultado & 0x0F) + 1;
              }

              showResultOnLEDs(resultado);

              String resultadoDecStr = getDecimalFrom1sComp(resultado);
              String resultadoBinStr = binaryToString4bit(resultado);

              client.println("<!DOCTYPE html><html>");
              client.println("<head><title>Resultado</title></head><body>");
              client.println("<h1>Resultado da ALU (1's Comp)</h1>");
              client.println("<p>Resultado decimal: " + resultadoDecStr + "</p>");
              client.println("<p>Resultado binário: " + resultadoBinStr + "</p>");
              client.println("<br><a href=\"/\">Nova Operação</a>");
              client.println("</body></html>");
            } 
            else {
              client.println("<!DOCTYPE html><html>");
              client.println("<head><title>ESP32 ALU Interface</title></head><body>");
              client.println("<form action=\"/calculate\" method=\"GET\">");
              
              client.println("<label for=\"opA\">Operand A (4-bit):</label>");
              client.println("<input type=\"text\" id=\"opA\" name=\"opA\" pattern=\"[01]{4}\" maxlength=\"4\" required><br><br>");
              
              client.println("<label for=\"opB\">Operand B (4-bit):</label>");
              client.println("<input type=\"text\" id=\"opB\" name=\"opB\" pattern=\"[01]{4}\" maxlength=\"4\" required><br><br>");
              
              client.println("<label for=\"operation\">Operation:</label>");
              client.println("<select id=\"operation\" name=\"operation\" required>");
              client.println("<option value=\"sum\">Sum (Add)</option>");
              client.println("<option value=\"sub\">Sub (Subtract)</option>");
              client.println("</select><br><br>");
              
              client.println("<input type=\"submit\" value=\"Send to ESP32\">");
              client.println("</form></body></html>");
            }
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}

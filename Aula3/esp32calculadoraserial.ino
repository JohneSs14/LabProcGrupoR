#include <Arduino.h>

const int LEDS[4] = {4, 5, 6, 7};

String getDecimalFrom1sComp(uint8_t binValue) {
  binValue &= 0x0F;
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
    digitalWrite(LEDS[i], (value >> i) & 1);
  }
}

uint8_t readBinary4bit(const char *prompt) {
  while (true) {
    Serial.print(prompt);
    while (Serial.available() == 0) { delay(10); }
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() != 4) {
      Serial.println("Erro: digite exatamente 4 bits (0/1).");
      continue;
    }
    bool valid = true;
    for (int i = 0; i < 4; i++) {
      if (input[i] != '0' && input[i] != '1') { valid = false; break; }
    }
    if (!valid) {
      Serial.println("Erro: apenas 0 e 1 sao permitidos.");
      continue;
    }
    Serial.println(input);
    return (uint8_t)strtol(input.c_str(), NULL, 2);
  }
}

char readOperation() {
  while (true) {
    Serial.print("Operacao (+ para soma, - para subtracao): ");
    while (Serial.available() == 0) { delay(10); }
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input == "+" || input == "sum") { Serial.println("+"); return '+'; }
    if (input == "-" || input == "sub") { Serial.println("-"); return '-'; }
    Serial.println("Erro: digite + ou -.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  for (int i = 0; i < 4; i++) {
    pinMode(LEDS[i], OUTPUT);
    digitalWrite(LEDS[i], LOW);
  }

  Serial.println("=== ALU 4-bit Complemento de Um (Serial) ===");
}

void loop() {
  uint8_t opA = readBinary4bit("Operando A (4 bits): ");
  uint8_t opB = readBinary4bit("Operando B (4 bits): ");
  char op = readOperation();

  uint8_t opB_internal = opB;
  if (op == '-') {
    opB_internal = (~opB) & 0x0F;  // complemento de um
  }

  uint16_t resultado = opA + opB_internal;

  // end-around carry (complemento de um)
  if (resultado > 15) {
    resultado = (resultado & 0x0F) + 1;
  }

  uint8_t res = resultado & 0x0F;

  showResultOnLEDs(res);

  Serial.println("-------------------------");
  Serial.print("A         = "); Serial.print(binaryToString4bit(opA));
  Serial.print(" ("); Serial.print(getDecimalFrom1sComp(opA)); Serial.println(")");
  Serial.print("B         = "); Serial.print(binaryToString4bit(opB));
  Serial.print(" ("); Serial.print(getDecimalFrom1sComp(opB)); Serial.println(")");
  Serial.print("Operacao  = "); Serial.println(op);
  Serial.print("Resultado = "); Serial.print(binaryToString4bit(res));
  Serial.print(" ("); Serial.print(getDecimalFrom1sComp(res)); Serial.println(")");
  Serial.println("-------------------------\n");
}

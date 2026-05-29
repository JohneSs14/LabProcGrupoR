const int LED1 = 26;
const int LED2 = 27;
const int LED3 = 32;
const int LED4 = 33;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

void loop() {
  digitalWrite(LED1, HIGH); // Turns the LED on
  digitalWrite(LED4, LOW);  // Turns the LED off
  delay(1000);                // Waits for 1 second
  digitalWrite(LED2, HIGH); // Turns the LED on
  digitalWrite(LED1, LOW);  // Turns the LED off
  delay(1000); 
  digitalWrite(LED3, HIGH); // Turns the LED on
  digitalWrite(LED2, LOW);  // Turns the LED off
  delay(1000);                // Waits for 1 second
  digitalWrite(LED4, HIGH); // Turns the LED on
  digitalWrite(LED3, LOW);  // Turns the LED off
  delay(1000); 
}

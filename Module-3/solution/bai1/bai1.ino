#define LED1 16
#define LED2 2
#define BUTTON 0

int cycle = 500;
int state = LOW;
boolean interruptFlag = false;

void ICACHE_RAM_ATTR handleInterrupt() {
  unsigned long t = millis();
  while (digitalRead(BUTTON) == LOW);
  if (millis() - t > 2000) {
    if (cycle == 500) {
      cycle = 1000;
    }
    else {
      cycle = 500;
    }
  }
  else {
    if (state == LOW) {
      state = HIGH;
    }
    else {
      state = LOW;
    }
  }
  interruptFlag = true;
}

void setup() {
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, state);
  pinMode(LED2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON), handleInterrupt, FALLING);
  Serial.begin(115200);
}

void loop() {
  if (interruptFlag) {
    digitalWrite(LED1, state);
    Serial.print(cycle);
    Serial.print(", ");
    if(state == LOW) {
      Serial.print("ON");
    }
    else {
      Serial.print("OFF");
    }
    Serial.println();
    interruptFlag = false;
  }
  digitalWrite(LED2, !digitalRead(LED2));
  delay(cycle);
}

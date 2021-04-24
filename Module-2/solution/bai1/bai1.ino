#define LED 16
#define BUTTON 0

int i = 0;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BUTTON, INPUT);
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(BUTTON) == LOW) {
    delay(50);
    while (digitalRead(BUTTON) == LOW);
    delay(50);
    i = i + 1;
    Serial.println(i);
  }

  if (digitalRead(LED) == LOW && i == 2) {
    digitalWrite(LED, HIGH);
    i = 0;
  }

  if (digitalRead(LED) == HIGH && i == 3) {
    digitalWrite(LED, LOW);
    i = 0;
  }
}

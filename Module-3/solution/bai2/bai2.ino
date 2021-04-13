#define LED 16
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(115200);
}

void loop() {
  if(Serial.available())
  {
    String text = Serial.readString();
    if(text == "LED_ON") {
      digitalWrite(LED, LOW);
    }
    else if(text == "LED_OFF") {
      digitalWrite(LED, HIGH);
    }
    else {
      Serial.println("Invalid String");
    }
  }
}

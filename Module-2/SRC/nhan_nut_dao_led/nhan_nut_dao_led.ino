void setup() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
  pinMode(0, INPUT);
  digitalWrite(16, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(0) == LOW) {
    delay(50);
    while(digitalRead(0) == 0);
    delay(50);

    digitalWrite(16, !digitalRead(16));
  }
  //-------------
// 10s  
  //--------------
}

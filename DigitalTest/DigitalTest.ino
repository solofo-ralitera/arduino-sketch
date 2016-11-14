void setup() {
  // put your setup code here, to run once:
  for(short i = 2; i <= 12; i++) { 
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(short i = 2; i <= 12; i++) { 
    digitalWrite(i, HIGH);
    delay(500);
    digitalWrite(i, LOW);
    delay(500);
  }

}

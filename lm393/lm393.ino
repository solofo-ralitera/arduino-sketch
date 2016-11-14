void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  delay(100);

}

void loop() {
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);
  delay(100);
  
  
}

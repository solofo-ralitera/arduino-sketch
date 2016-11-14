void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(2, OUTPUT);
  
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);

  lowAll();
}

void loop() {
  // put your main code here, to run repeatedly:

  setA();
  delay(3000);

  setB();
  delay(3000);
  
}

void lowAll() {
  digitalWrite(12, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(11, LOW);
}

void setA() {
  lowAll();
  digitalWrite(3, HIGH);
  digitalWrite(11, HIGH);
}

void setB() {
  lowAll();
  digitalWrite(12, HIGH);
  digitalWrite(2, HIGH);
}


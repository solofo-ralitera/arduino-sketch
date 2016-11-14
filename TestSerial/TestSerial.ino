void setup() {
  Serial.begin(9600);
}
void loop() {
    Serial.write("A");
    Serial.write("\n");
    delay(1000);
}


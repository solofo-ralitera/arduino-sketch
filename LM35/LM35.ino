
#define PIN_CATPEUR A7

// Timer pour loop / sec
unsigned long timer = millis();
unsigned long mils;


void setup()
{
  Serial.begin(9600);

}

void loop()
{
  // Chekc for timer to loop chaque seconde
  mils = millis();

  if (mils - timer >= 1000) {
    timer = mils;

    float Temp = (analogRead(PIN_CATPEUR) * 5. / 1023 * 100);
    
    Serial.println(Temp);
  }
}

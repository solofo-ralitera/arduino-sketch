// Read frequence et % cyclique from NE555

int inPin = 7;
int val = 0;

unsigned long timer;

unsigned int timerHigh;
unsigned int timerLow;
unsigned int timerCyle;

float frequence;
float rapportCyclique;

float mFrequence;
float mRapportCyclique;

String sFrequence = "";
String sRapportCyclique = "";

float moyenne = 1.;

void setup() {
  pinMode(inPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  mFrequence = 0.;
  mRapportCyclique = 0.;
  for(byte i = 0; i < moyenne; i++) {
    readPulse();
    mFrequence += frequence;
    mRapportCyclique += rapportCyclique;
    //delay(20);
  }
  mFrequence = mFrequence / moyenne;
  mRapportCyclique = mRapportCyclique / moyenne;

  sFrequence = String(mFrequence) + "Hz";
  sRapportCyclique = String(mRapportCyclique) + "%";
  Serial.println(sRapportCyclique);
  Serial.println(sFrequence);
  Serial.println("");
}

void readPulse() {
  val = digitalRead(inPin);

  // Calcul tmps haut
  timer = millis();
  while(val == 1) {
    // wait tant que high
    val = digitalRead(inPin);
  }
  timerHigh = millis() - timer;

  //Temps bas
  while(val == 0) {
    // wait tant que low
    val = digitalRead(inPin);
  }
  timerCyle = millis() - timer;
  timerLow = timerCyle - timerHigh;

  rapportCyclique = 100. * timerHigh / timerCyle;
  frequence = 1000. / timerCyle;
  /*
  float ret[] {rapportCyclique, frequence};
  return ret;
  */
}


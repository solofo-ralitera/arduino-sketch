
const int microphonePin= 0; //the microphone positive terminal will connect to analog pin A0 to be read

// PWM pin for led
const int ledPin = 12;

//the variable that will hold the value read from the microphone each time
int sample;

void setup() {
  //pinMode (ledPin, OUTPUT);//sets digital pin 13 as output
  Serial.begin(9600); //sets the baud rate at 9600 so we can check the values the microphone is obtaining on the Serial Monitor
}

void loop(){
  sample= analogRead(microphonePin); //the arduino takes continuous readings from the microphone
  int pwm = map(sample, 0, 1023, 0, 255);
  analogWrite(ledPin, pwm);
  Serial.println(pwm);
  //delay(1000);
  /*
  if (sample > threshold)
  {
  digitalWrite (ledPin, HIGH); //if the reading is greater than the threshold value, LED turns on
  delay (500); //LED stays on for a half a second
  digitalWrite (ledPin, LOW); //LED turns off
  }
  else{ digitalWrite(ledPin, LOW); }
  */
}





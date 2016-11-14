void setup() {
  Serial.begin(9600);
}
long CurR = 0;
void loop() {
  
  if('R'=='R')
  {
    // tension Ur en mv
    // valeur de la résistance R
    long R = 0;
    long U;
    for(int i=0; i <= 10; i++) {
      U = analogRead(1);
      U *= 5000.;
      U /= 1023.;  
      R += (U * 10000.) / (5000. - U);    
    }
    R = R / 10.;
    // envoi de la valeur de R sur le port serie
    if(CurR == R) {
      
    }else {
      Serial.println(R);
      Serial.println("");
      CurR = R;
    }
    delay(1000);
  }
  
  //int btn = map(analogRead(0), 0, 986, 1, 27);

  //Serial.println(btn);
  //delay(1000);
}


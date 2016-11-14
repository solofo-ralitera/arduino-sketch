#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(11,13,10,1);

int count = 1;

#define bip 2

void setup()
{
    lc.shutdown(0,false);
    lc.setIntensity(0,5);
    lc.clearDisplay(0);

    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    
}

void loop()
{
    String t = "50L0";
    lc.setString(0, t);
    delay(100);
    /*
    lc.setDigit(0,0,(int)(t[0]-'0'),true);
    lc.setDigit(0,1,(int)(t[1]-'0'),false);
    lc.setDigit(0,2,(int)(t[2]-'0'),false);
    lc.setDigit(0,3,(int)(t[3]-'0'),false);
    delay(1000);
    lc.shutdown(0,true);
    lc.shutdown(0,false);

    if(count % 10 == 0) {
      digitalWrite(bip, HIGH);
      delay(50);
      digitalWrite(bip, LOW);
      delay(50);

      digitalWrite(bip, HIGH);
      delay(50);
      digitalWrite(bip, LOW);
      delay(50);

      digitalWrite(bip, HIGH);
      delay(50);
      digitalWrite(bip, LOW);
      delay(50);

      digitalWrite(bip, HIGH);
      delay(50);
      digitalWrite(bip, LOW);
      delay(50);

      digitalWrite(2, HIGH);
      delay(1000);
      digitalWrite(2, LOW);
      delay(500);
    }
    */
}
                                                                                                                                                                                                                                                                                                                                                                                                                                      

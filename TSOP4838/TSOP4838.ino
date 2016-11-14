/* 
 source: www.electroschematics.com
 You'll need to change the led pins and the codes 
 accordingly to your configuration and IR remote
 */
  // Recepteur infrarouge
#include <IRremote.h>

int RECV_PIN = 7; // the pin where you connect the output pin of TSOP4838

/* the initial state of LEDs is OFF (zero) 
the first zero must remain zero but you can 
change the others to 1's if you want a certain
led to light when the board is powered 
*/

#define code1  63495 // code received from button A
#define code2  30855 // code received from button B
#define code3  22695 // code received from button C

IRrecv irrecv(RECV_PIN);


decode_results results;
void setup()
{
  Serial.begin(9600);   // you can comment this line
  irrecv.enableIRIn();  // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume();
  }else {
    //Serial.println("Off");
  }
  //delay(1000);
}


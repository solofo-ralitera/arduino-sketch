#include <Servo.h> 
 
Servo myservo; // create servo object to control a servo 
// a maximum of eight servo objects can be created 
 
int pos = 0; // variable to store the servo position 
 
void setup() 
{ 
  Serial.begin(9600);
  
  myservo.attach(9); // attaches the servo on pin 9 to the servo object 
} 

void loop() 
{ 
  // Read potar
  int tmppos = map(analogRead(A7), 0, 1023, 1, 179);
  if(tmppos != pos) {
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    pos = tmppos;
    delay(15); // waits 15ms for the servo to reach the position   
  }
}

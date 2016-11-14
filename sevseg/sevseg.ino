//http://blog.rastating.com/using-a-7-segment-led-display-w-arduino-uno/

#include "SevSeg2Digit.h"

#define NUM_DIGIT 2
#define D1 2
#define D2 3
#define SEG_A 5
#define SEG_B 7
#define SEG_C 8
#define SEG_D 10
#define SEG_E 9
#define SEG_F 6
#define SEG_G 4
#define SEG_DP 4

byte DigitPins[NUM_DIGIT];
byte SegmentPins[8];

SevSeg2Digit sevseg;

unsigned long timer;
int CentSec=0;

void setup()  
{
  Serial.begin(9600);
  
  timer=millis();
  
  sevseg.Begin(1, D1, D2, SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_DP);
  
  /*
  DigitPins[0] = D1;
  DigitPins[1] = D2;

  SegmentPins[0] = SEG_A;
  SegmentPins[1] = SEG_B;
  SegmentPins[2] = SEG_C;
  SegmentPins[3] = SEG_D;
  SegmentPins[4] = SEG_E;
  SegmentPins[5] = SEG_F;
  SegmentPins[6] = SEG_G;
  SegmentPins[7] = SEG_DP;
  
  clear();
  */
  
}

void loop()  
{  
  //Produce an output on the display
  sevseg.PrintOutput();

  //Check if 10ms has elapsed
  unsigned long mils=millis();
  if (mils-timer>=1000) {
    timer=mils;
    CentSec++;
    if (CentSec==10000) { // Reset to 0 after counting for 100 seconds.
      CentSec=0;
    }
    //Update the number to be displayed, with a decimal
    //place in the correct position.
    sevseg.NewNum(CentSec,(byte) 2);
  }
}

/**
 * Clear screen
 */
void clear() {
  //Set digit pin and off
  for (byte d=0; d < sizeof(DigitPins); d++) {
    pinMode(DigitPins[d], OUTPUT);
    digitalWrite(DigitPins[d], LOW);
  }  
  
  //Set segment pin and off
  for (byte d=0; d < sizeof(SegmentPins); d++) {
    pinMode(SegmentPins[d], OUTPUT);
    digitalWrite(SegmentPins[d], HIGH);
  }
}

/**
 * Display a number
 */
void display(int num) {
  clear();
  
  byte digit1 = num % 10;
  byte digit2 = num / 10;

  displayDigit(D1, digit1);
  //displayDigit(D2, digit2);
  
  //Serial.println(digit1);
  //Serial.println(digit2);
}

/**
 * Display digit
 */
void displayDigit(byte digitPin, byte number) {
  digitalWrite(digitPin, HIGH);
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
  
  switch(number) {
    case 0:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_B, LOW);
      break;
    case 1:
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, LOW);
      break;
    case 2:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_G, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_D, LOW);
      break;
    case 3:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_G, LOW);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, LOW);
      break;
    case 4:
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_G, LOW);
      digitalWrite(SEG_C, LOW);
      break;
    case 5:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, LOW);
      break;
    case 6:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_C, LOW);
      break;
    case 7:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, LOW);
      break;
    case 8:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_E, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      break;
    case 9:
      digitalWrite(SEG_A, LOW);
      digitalWrite(SEG_B, LOW);
      digitalWrite(SEG_C, LOW);
      digitalWrite(SEG_D, LOW);
      digitalWrite(SEG_F, LOW);
      digitalWrite(SEG_G, LOW);
      break;
    default:
      digitalWrite(SEG_A, HIGH);
      digitalWrite(SEG_B, HIGH);
      digitalWrite(SEG_C, HIGH);
      digitalWrite(SEG_D, HIGH);
      digitalWrite(SEG_E, HIGH);
      digitalWrite(SEG_F, HIGH);
      digitalWrite(SEG_G, HIGH);
      break;
    
  }
  
}


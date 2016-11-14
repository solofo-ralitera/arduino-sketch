#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

/*
** Example Arduino sketch for SainSmart I2C LCD Screen 16x2
** based on https://bitbucket.org/celem/sainsmart-i2c-lcd/src/3adf8e0d2443/sainlcdtest.ino
** by
** Edward Comer
** LICENSE: GNU General Public License, version 3 (GPL-3.0)

** This example uses F Malpartida's NewLiquidCrystal library. Obtain from:
** https://bitbucket.org/fmalpartida/new-liquidcrystal 

** Modified Ã¢â‚¬â€œ Ian Brennan ianbren at hotmail.com 23-10-2012 to support Tutorial posted to Arduino.cc

** Written for and tested with Arduino 1.0
**
** NOTE: Tested on Arduino Uno whose I2C pins are A4==SDA, A5==SCL

*/
/**
 * Branchement
 * 
 * I2C      GND   VCC   SDA   SCL 
 * Uno      GND   5V    A4    A5
 * Nano     Idem
 * 
 * 
 */
 
 //https://github.com/bogde/HX711
 
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <I2CScanner.h>
#include <HX711.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define BACKLIGHT_PIN     3

I2CScanner i2scaner;

//LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
LiquidCrystal_I2C  lcd  ( i2scaner.GetAdress(),   2,     1,     0,     4,     5,      6,    7     );
//LiquidCrystal_I2C  lcd  ( 0x3F,   2,     1,     0,     4,     5,      6,    7     );

HX711 scale(A0, 13);
float Calibrage;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(2);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(9600);
  
  lcd.begin (16,2); //  <<----- My LCD was 16x2
  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(125);

  lcd.home(); // go home (cursor 0,0)

  // Init scale
  lcd.print("raim sy 8...");  
  while(! scale.is_ready()) {
    lcd.blink();
    lcd.cursor();
    delay(10);
  }
  calibrateScale();

  // Home message
  lcd.setCursor (0,0);
  lcd.print("Lanja miakatra           ");  
  lcd.noBlink();
  lcd.noCursor();
}

void loop()
{
  float value = getScale();
  
  lcd.setCursor (0,0);
  Serial.println(value);
  lcd.print(value, 3);
  lcd.print(" g              ");

  lcd.setCursor (0,1);
  sensors.requestTemperatures();
  lcd.print(sensors.getTempCByIndex(0), 2);
  lcd.print(" C              ");
  
  delay(50);
}

float getScale() {
  float value = scale.get_units(20);
  value *= -1;
  return value;
}

void calibrateScale() {
  float startCalibration = 423.56; // Start Calibration 428.56
  float value;
  
  scale.set_scale(startCalibration);
  scale.tare();
  
  delay(10);

  while(true) {
    value = getScale();
    
    // scale ok
    if(value < 0.05 && value >= 0.) {
      return ;
    }else {
      startCalibration = startCalibration - 0.25;
      scale.set_scale(startCalibration);
      scale.tare();
      delay(10);
    }
  }
}


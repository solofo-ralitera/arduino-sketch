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

** Modified â€“ Ian Brennan ianbren at hotmail.com 23-10-2012 to support Tutorial posted to Arduino.cc

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
 
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <I2CScanner.h>

#define BACKLIGHT_PIN     3

I2CScanner i2scaner;

//LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
//LiquidCrystal_I2C  lcd  ( i2scaner.GetAdress(),   2,     1,     0,     4,     5,      6,    7     );
LiquidCrystal_I2C  lcd  ( 0x3F,   2,     1,     0,     4,     5,      6,    7     );

void setup()
{
  lcd.begin (16,2); //  <<----- My LCD was 16x2

  
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(125);

  lcd.home (); // go home

  lcd.print("Hello");  
  lcd.setCursor (0,1);
  lcd.print("   World !!!"); 
}

void loop()
{

}

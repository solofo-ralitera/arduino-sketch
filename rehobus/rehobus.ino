/*
  Rehobus 1 sortie 12v controlé par relais
*/

#include <Wire.h>
#include "LedControl.h"

/*** PIN CONFIGURATION ***/
// Pin analog du capteur LM35
#define PIN_CATPEUR A7

// PIN digital du FAN Molex
#define PIN_FAN_MOLEX 10

// PIN digital du FAN PWM
#define PIN_FAN_PWM 9
#define PIN_FAN_PWM_TACH 2
#define PIN_FAN_PWM_PWM 3

// PIN DIGITAL du FAN 12v
#define PIN_FAN_12V 8

// Pin digital infrarouge TSOP4838
#define PIN_IRRECEIVER 11

/*** CONFIG ***/
// °C to activate relais
#define TEMP_START 25


/**** MODULE CONFIGURATION ****/
// DS3231 RTC
#define RTC_MODULE true

// Use LCD 16x2
#define DISPLAY_LCD false

// Use OLED SPI 128x64
#define DISPLAY_OLED true

// IR receiver
#define IR_RECEIVER false

/**** INCLUDE CONFIG ***/
#if DISPLAY_OLED == true
  #include <U8glib.h>
  // Affichage oled
  U8GLIB_SH1106_128X64 oled(4, 5, 6, 7);   // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7
  
  void oledDraw(const char *str, int x = 0, int y = 10) {
    // graphic commands to redraw the complete screen should be placed here
    //oled.setFont(u8g_font_5x7);
    //u8g.setFont(u8g_font_osb21);
    oled.drawStr( x, y, str);
  }
  void oledDraw(String str, int x = 0, int y = 10) {
    const char* tmp = (const char*) str.c_str();
    // graphic commands to redraw the complete screen should be placed here
    //oled.setFont(u8g_font_6x10);
    //u8g.setFont(u8g_font_osb21);
    oled.drawStr( x, y, tmp);
  }
#endif

#if RTC_MODULE == true
  #include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
  #include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#endif

#if DISPLAY_LCD == true
  #include <LCD.h>
  #include <LiquidCrystal_I2C.h>
  // Affichage sur LCD 16x2
  LiquidCrystal_I2C  lcd(0x3F,   2,     1,     0,     4,     5,      6,    7     );
#endif


// Init Ir
#if IR_RECEIVER == true
  #include <IRremote.h>
  IRrecv irrecv(PIN_IRRECEIVER);
  decode_results results;
#else
  bool irrecv;
#endif

// 7-SEG
LedControl sevSeg = LedControl(11,13,10,1);

/**************************************/

// Temperature moyenne de la mesure
int Temp = 0;
// Relais fermé ou pas
bool RelaisActif = false;

// Timer pour loop / sec
unsigned long timer = millis();
unsigned long mils;

// Fan pwm pulse
unsigned long pulseDuration;

// String display rtc date/time

// Affichage OLED strin par line
String oled1 = "";
String oled2 = "";
String oled3 = "";
String oled4 = "";
String oled5 = "";
String oled6 = "";


void eeprom_i2c_write(byte address, byte from_addr, byte data) {
  Wire.beginTransmission(address);
  Wire.write(from_addr);
  Wire.write(data);
  Wire.endTransmission();
}

byte eeprom_i2c_read(int address, int from_addr) {
  Wire.beginTransmission(address);
  Wire.write(from_addr);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);
  if(Wire.available())
    return Wire.read();
  else
    return 0xFF;
}

void setup()
{
  Serial.begin(9600);
  
  // Init Relais sur le Pin 2
  // Pour ULN2003 (HIGHT fermé, LOW ouvert)
  if (PIN_FAN_MOLEX != false) {
    pinMode(PIN_FAN_MOLEX, OUTPUT);
    digitalWrite(PIN_FAN_MOLEX, LOW);
  }

  if (PIN_FAN_MOLEX != false) {
    pinMode(PIN_FAN_12V, OUTPUT);
    digitalWrite(PIN_FAN_12V, LOW);
  }


  // Pour ULN2003 (HIGHT fermé, LOW ouvert)
  if (PIN_FAN_PWM != false) {
    pinMode(PIN_FAN_PWM, OUTPUT);
    digitalWrite(PIN_FAN_PWM, LOW);

    // pulse tachometer
    pinMode(PIN_FAN_PWM_TACH, INPUT);
    digitalWrite(PIN_FAN_PWM_TACH,HIGH);
    
    //analogWrite(PIN_FAN_PWM_PWM, 255);
  }


  //Serial.begin(9600);

  /*
    if(DISPLAY_LCD) {
    lcd.begin (16,2);
    // Switch on the backlight
    //lcd.setBacklightPin(3, NEGATIVE);
    }
  */

  // Start the IR TSOP4838 receiver
  if (IR_RECEIVER == true) {
    //irrecv.enableIRIn();
  }

  if (DISPLAY_OLED != false) {
    // assign default color value
    if ( oled.getMode() == U8G_MODE_R3G3B2 ) {
      oled.setColorIndex(255);     // white
    }
    else if ( oled.getMode() == U8G_MODE_GRAY2BIT ) {
      oled.setColorIndex(3);         // max intensity
    }
    else if ( oled.getMode() == U8G_MODE_BW ) {
      oled.setColorIndex(1);         // pixel on
    }
    else if ( oled.getMode() == U8G_MODE_HICOLOR ) {
      oled.setHiColorByRGB(255, 255, 255);
    }
    oled.setFont(u8g_font_5x7);
    oled.setFontRefHeightExtendedText();
  }

  if (RTC_MODULE == true) {
    setSyncProvider(RTC.get);
  }
  if(year() == 1970) {
    // Reccup temps dans eeprom
    /**
    int year1 = (int) eeprom_i2c_read(B01010000, 0);
    int year2 = (int) eeprom_i2c_read(B01010000, 1);
    Serial.println(hr);
    //setTime(int hr,int min,int sec,int day, int month, int yr);
    //setTime(int hr,int min,int sec,int day, int month, int yr);
    */
  }
  
  sevSeg.shutdown(0,false);
  sevSeg.setIntensity(0,5);
  sevSeg.clearDisplay(0);
}

void loop()
{
  // Check for timer to loop chaque seconde
  mils = millis();
  
  if (mils - timer >= 1000) {
    timer = mils;

    /// Loop pour moyenne
    Temp = 0;
    for (int i = 1; i <= 10; i++) {
      Temp += (analogRead(PIN_CATPEUR) * 5. / 1023 * 100);
    }
    Temp = Temp / 10;

    //Serial.println(Temp);

    RelaisActif = false;

    if (Temp > TEMP_START) {
      RelaisActif = true;
    }

    // Ferme le relais si t >
    // Si relais : HIGH : ouvert, LOW : fermé
    // Si ULN2003 : HIGH : ferme, LOW : ouvert
    if (RelaisActif) {
      if (PIN_FAN_MOLEX != false) {
        /**
           AnalogWrite su ULN2003 (1fan / 2fan)
            10 =>   5.45
            15 =>   7.46
            20 =>   9.15
            25 =>   10.13   7.10
            30 =>   10.36   8.24
            35 =>   10.52   9.17
            40 =>   10.63   9.80
            45 =>   10.71   10.09
            50 =>   10.78   10.26
            60 =>   10.88   10.46
            70 =>   10.95   10.60
            80 =>   11.00   10.70
            90 =>   11.04   10.76
            100 =>  11.07   10.82
            125 =>  11.14   10.94
            150 =>  11.18   11.02
            175 =>  11.22   11.07
            200 =>  11.24   11.12
            225 =>  11.27   11.15
            250 =>  11.28   11.18
            255 =>  11.28   11.18
        */
        //digitalWrite(PIN_FAN_MOLEX, HIGH);
        analogWrite(PIN_FAN_MOLEX, 175);
      }
      if (PIN_FAN_PWM != false) {
          // TODO gestion PWM
          // Value du pwn entre 150 et 255 proportionnel à temp TEMP_START et TEMP_START + 25%
          int bMAx = TEMP_START * 1.25;
          int pwm = map(min(Temp, bMAx), TEMP_START, bMAx, 100, 255);
          analogWrite(PIN_FAN_PWM_PWM, pwm);
          
          //Active fan
          digitalWrite(PIN_FAN_PWM, HIGH);
      }
      
      if (PIN_FAN_12V != false) {
        digitalWrite(PIN_FAN_12V, HIGH);
      }      
      
    } else {
      if (PIN_FAN_MOLEX != false) {
          // Stop ULN2003
          digitalWrite(PIN_FAN_MOLEX, LOW);
      }
      
      if (PIN_FAN_PWM != false) {
          // Stop ULN2003
          digitalWrite(PIN_FAN_PWM, LOW);
      }

      if (PIN_FAN_12V != false) {
          // Stop ULN2003
          digitalWrite(PIN_FAN_12V, LOW);
      }
      
      //delay(1000);
    }

    if (PIN_FAN_PWM != false) {
        // TODO : test si PIN_FAN_PWM HIGH
        // READ FAN RPM
        pulseDuration = pulseIn(PIN_FAN_PWM_TACH, LOW);
        int frequency =  1000000 / pulseDuration;
        int rpm1 = frequency / 2*60;
        int pct = (rpm1 / 4080.) * 100;
        /*
        Serial.print("pulse duration:");
        Serial.println(pulseDuration);

        Serial.print("time for full rev. (microsec.):");
        Serial.println(pulseDuration*2);
        Serial.print("freq. (Hz):");
        Serial.println(frequency/2);
        */
        oled2 = "Fan1 : " + String(rpm1)  + "rpm - " + String(pct) + "%";
        //set7SegDigit(lc, rpm1);
    }
        
    if (RTC_MODULE == true) {
      oled6 = "     " + String(day()) + "/" + String(month()) + "/" + String(year()) + " " + String(hour()) + ":" + String(minute()) + ":" + String(second());
    }

    sevSeg.setString(0, String(Temp));
  }
  /*
    // Display for lcd
    if(DISPLAY_LCD) {
    lcd.home ();
    String tmp = "";
    // Print 27°C  Off
    tmp = tmp + Temp + (char)223 + "C  " + ((RelaisActif)? "On" : "Off") + "      ";
    lcd.print(tmp);
    }
  */

  //Display for Oled
  //https://github.com/rydepier/Arduino-OLED-Clock/blob/master/Arduino-OLED-Clock%20using%20U8GLIB%20library.ino
  if (DISPLAY_OLED != false) {
    oled.firstPage();
    oled1 = "";
    oled1 = "Temp " + String(Temp) + (char)126 + "C";
    oled1 += "    Fan ";
    oled1 += ((RelaisActif) ? "On" : "Off");
    
    // TSOP4838 receiver
    String rc = "";
    /**
      if (PIN_IRRECEIVER != false && irrecv.decode(&results)) {
      //rc = getCanalSatRC((long )results.value);
      rc = String(results.value);
      //Serial.println(rc);
      irrecv.resume();
      }
    */
    do {
      oledDraw(oled1);
      oledDraw(oled2, 0, 20);
      oledDraw(oled3, 0, 30);
      oledDraw(oled4, 0, 40);
      oledDraw(oled5, 0, 50);
      oledDraw(oled6, 0, 60);
      /**
      oledDraw(tmp);
      oledDraw(tmpFan, 60);
      oledDraw(rc, 0, 40);
      oledDraw(rtcTime, 0, 60);
      */
    } while ( oled.nextPage() );
  }
  // TSOP4838 receiver
  else if (DISPLAY_LCD != false) {
    /*
      if(PIN_IRRECEIVER != false && irrecv.decode(&results)) {
      //Serial.println(results.value);
      //String rc = getCanalSatRC((long )results.value);
      irrecv.resume();
      // Affiche sur le lcd
      lcd.setCursor (0,1);
      lcd.print(results.value);
      lcd.print("        ");
      }
    */
  }

}



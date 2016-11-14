// OLed

#include <SPI.h>
#include "SH1106_SPI.h"
#define USE_FRAME_BUFFER

#ifdef USE_FRAME_BUFFER
  SH1106_SPI_FB lcd;
#else
  SH1106_SPI lcd;
#endif
void setup(void)
{
  lcd.begin();

}

int i;

void loop() {

  lcd.print(F("Loading Bitmap....."));              //oled display
  lcd.renderAll(); 
  delay(1000);
  //lcd.clear();                                     // clears the screen and buffer
  //lcd.writeBitmap(rhy_128x40,4,1,128, 5);          //drawing bitmap
  //lcd.renderAll(); 
}


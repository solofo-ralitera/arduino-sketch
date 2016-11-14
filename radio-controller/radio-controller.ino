#include <U8glib.h>

#define BUTTON_PIN A0
#define TRANSMITTER_PIN 2
#define RECEIVER_PIN 3

// Btn code
#define BTN_BOTTOM_LEFT     1
#define BTN_BOTTOM_CENTRE       2
#define BTN_BOTTOM_RIGHT        3
#define BTN_RIGHT_BOTTOM        4
#define BTN_RIGHT_TOP       5
#define BTN_LEFT_TOP        6
#define BTN_NONE        0

// Text button
#define TEXT_BOTTOM_LEFT "aaa"
#define TEXT_BOTTOM_CENTER "acq"
#define TEXT_BOTTOM_RIGHT "bbb"

U8GLIB_SH1106_128X64 oled(4, 5, 6, 7);   // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7
String oledStr1 = "Hello world !!!";
String oledStr2 = "";
String oledStr3 = "";

// Current activ item
short CurrentChannel = 1;

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

short getBtn() {
  short U = analogRead(BUTTON_PIN);
  //oledStr3 = (String) U;
  // BTN BOTTOM-LEFT
  if(U >= 505 && U <= 515) {
    return BTN_BOTTOM_LEFT;
  }
  // BTN BOTTOM-CENTRE
  else if(U >= 813 && U <= 823) {
    return BTN_BOTTOM_CENTRE;
  }
  // BTN BOTTOM-RIGHT
  else if(U >= 675 && U <= 685) {
    return BTN_BOTTOM_RIGHT;
  }
  // BTN RIGHT-BOTTOM
  else if(U >= 762 && U <= 772) {
    return BTN_RIGHT_BOTTOM;
  }
  // BTN RIGHT-TOP
  else if(U >= 847 && U <= 857) {
    return BTN_RIGHT_TOP;
  }
  // BTN LEFT-TOP
  else if(U <= 10) {
    return BTN_LEFT_TOP;
  }
  return BTN_NONE;
}

void setup() {
  // put your setup code here, to run once:
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

void loop() {
  // catch button 
  switch(getBtn()) {
    case BTN_BOTTOM_LEFT:
      oledStr2 = "BTN_BOTTOM_LEFT";
      break;
    case BTN_BOTTOM_CENTRE:
      oledStr2 = "BTN_BOTTOM_CENTRE";
      break;
    case BTN_BOTTOM_RIGHT:
      oledStr2 = "BTN_BOTTOM_RIGHT";
      break;
    case BTN_RIGHT_BOTTOM:
      oledStr2 = "BTN_RIGHT_BOTTOM";
      break;
    case BTN_RIGHT_TOP:
      oledStr2 = "BTN_RIGHT_TOP";
      break;
    case BTN_LEFT_TOP:
      // Swith channel
      // TODO
      CurrentChannel++;
      break;
    case BTN_NONE:
    default:
      oledStr2 = "BTN_NONE";
      break;
  }

  
  //display
  oled.firstPage();
  do {
    // Main text
      // Top text (title)
    oledStr1.toUpperCase();
    oledDraw(".:: "+oledStr1+" ::.", 15, 10);
      // Second line
    oledDraw(oledStr2, 10, 20);
      // Third line
    oledDraw(oledStr3, 10, 28);

    // text for btn left
    oledDraw("CH" + (String) CurrentChannel, 0, 10);

    // text for btn bottom
    oledDraw(TEXT_BOTTOM_LEFT, 0, 63);
    oledDraw(TEXT_BOTTOM_CENTER, 57, 63);
    oledDraw(TEXT_BOTTOM_RIGHT, 114, 63);
    
  } while ( oled.nextPage() );  

}

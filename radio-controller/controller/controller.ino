#include <U8glib.h>
#include <VirtualWire.h>

#define BUTTON_PIN      A0
#define TRANSMITTER_PIN 2
#define RECEIVER_PIN    3

// Btn code
#define BTN_BOTTOM_LEFT     1
#define BTN_BOTTOM_CENTRE   2
#define BTN_BOTTOM_RIGHT    3
#define BTN_RIGHT_BOTTOM    4
#define BTN_RIGHT_TOP       5
#define BTN_LEFT_TOP        6
#define BTN_NONE            0

// Text button
String TEXT_BOTTOM_LEFT = "";
String TEXT_BOTTOM_CENTER = "";
String TEXT_BOTTOM_RIGHT = "";

U8GLIB_SH1106_128X64 oled(4, 5, 6, 7);   // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7
String oledStr1 = "Hello world !!!";
String oledStr2 = "";
String oledStr3 = "";

// Current activ item for BTN_LEFT_TOP
byte CurrentChannel = 0;
char ChannelList[] = {0, 49, 50, 51, 52, 53};

// Current / previous pressed button
short CurrentButton = -1;

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
  if (U >= 505 && U <= 515) {
    return BTN_BOTTOM_LEFT;
  }
  // BTN BOTTOM-CENTRE
  else if (U >= 813 && U <= 823) {
    return BTN_BOTTOM_CENTRE;
  }
  // BTN BOTTOM-RIGHT
  else if (U >= 675 && U <= 685) {
    return BTN_BOTTOM_RIGHT;
  }
  // BTN RIGHT-BOTTOM
  else if (U >= 762 && U <= 772) {
    return BTN_RIGHT_BOTTOM;
  }
  // BTN RIGHT-TOP
  else if (U >= 847 && U <= 857) {
    return BTN_RIGHT_TOP;
  }
  // BTN LEFT-TOP
  else if (U <= 10) {
    return BTN_LEFT_TOP;
  }
  return BTN_NONE;
}

void resetTxtBtn() {
  oledStr1 = "";
  oledStr2 = "";
  oledStr3 = "";

  TEXT_BOTTOM_LEFT = "";
  TEXT_BOTTOM_CENTER = "";
  TEXT_BOTTOM_RIGHT = "";  
  
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

  // Init transmitter
  vw_set_ptt_inverted(true); //
  vw_set_rx_pin(RECEIVER_PIN);
  vw_set_tx_pin(TRANSMITTER_PIN);
  vw_setup(4000);// speed of data transfer Kbps
  vw_rx_start();
}

// Transmit btn pressed
void transmitSignal() {
  char charBuf[50];
  String s = String(CurrentChannel) + "" + String(CurrentButton);
  s.toCharArray(charBuf, 50);
  vw_send((uint8_t *)charBuf, strlen(charBuf));
  vw_wait_tx();
}

char Message[VW_MAX_MESSAGE_LEN];
void receiveSignal() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {
    short i;
    for (i = 0; i < buflen; i++) {
      if(i == 0) {
        // not active module
        if(buf[i] != ChannelList[CurrentChannel]) {
          return;
        }
      }else {
        oledStr2 = "Ici " + (String) char(buf[i]);
        Message[i] = char(buf[i]);
      }
    }
    Message[buflen] = '\0';
    manageSignal(Message);
  }
}

void manageSignal(char* message) {
  char dlm[] = "|";
  char *pch = strtok(message, "|");
  while ( pch != NULL ) {
      //oledStr2 += "-" + String(pch);
      pch = strtok (NULL, dlm);
  }
}

void loop() {

  // Skip si maintain btn
  if(CurrentButton == getBtn()) {
    // TODO : Btn maintained
  }
  else {
    CurrentButton = getBtn();
    // catch button
    switch(CurrentButton) {
      case BTN_BOTTOM_LEFT:
        

        // Transmit signal
        transmitSignal();
        break;
      case BTN_BOTTOM_CENTRE:
        
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_BOTTOM_RIGHT:
        
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_RIGHT_BOTTOM:
        
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_RIGHT_TOP:
        
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_LEFT_TOP:
        // Swith channel
        // TODO init choosed chanel
        CurrentChannel++;
        if(CurrentChannel >= sizeof(ChannelList)) CurrentChannel = 0;
        resetTxtBtn();
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_NONE:
      default:
        
        // Transmit signal
        transmitSignal();
        break;
    }
  }

  // Receive signal
  receiveSignal();
  
  //display
  oled.firstPage();
  do {
    // Main text
    // Top text (title)
    oledStr1.toUpperCase();
    oledDraw(oledStr1, 20, 10);
    // Second line
    oledDraw(oledStr2, 10, 20);
    // Third line
    oledDraw(oledStr3, 10, 28);

    // text for btn left
    oledDraw("C" + (String) CurrentChannel, 0, 10);

    // text for btn bottom
    oledDraw(TEXT_BOTTOM_LEFT, 0, 63);
    oledDraw(TEXT_BOTTOM_CENTER, 57, 63);
    oledDraw(TEXT_BOTTOM_RIGHT, 114, 63);

  } while ( oled.nextPage() );
}

/********************
 * Controller with 7 buttons
 * Btn 1 (Left top CX): 
 *  - short press : switch between modules
 *  - long press : reload current module
 * Btn 2 (Left bottom |):
 *  - short press : nextpage
 *  - long press : oled on/off
 * Other buttons : specific for each modules
 *    Each press transimt message with following format:
 *      2 digits:
 *          digit 1 : current modules code
 *          digit 2 : pressed button code
 * Message received format:
 *    digit 1 : module code which send the signal
 *    alphanum char (len 1):
 *      - m : code for initing module interface
 *          Following format : m|MODULE TITLE+"|menu1|menu2|menu3|menu4|menu5
 *      - a : to display message between line 2-6
 *          Following format : a|line2|line3|line4|line5|line6
 */
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
#define BTN_LEFT_BOTTOM     7
#define BTN_NONE            0

// Text button
String TEXT_BOTTOM_LEFT = "";
String TEXT_BOTTOM_CENTER = "";
String TEXT_BOTTOM_RIGHT = "";
String TEXT_RIGHT_BOTTOM = "";
String TEXT_RIGHT_TOP = "";

U8GLIB_SH1106_128X64 oled(4, 5, 6, 7);   // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7
String oledStr1 = "";
String oledStr2 = "";
String oledStr3 = "";
String oledStr4 = "";
String oledStr5 = "";
String oledStr6 = "";
String oledStr7 = "";
String oledStr8 = "";
String oledStr9 = "";
String oledStr10 = "";
String oledStr11 = "";

// Current activ item for BTN_LEFT_TOP
byte CurrentChannel = 0;
byte CurrentChannetInited = 0;
const char ChannelList[] = {0, 49, 50, 51, 52, 53};

// Oled current page displayed
byte currentPage = 1;

// Current / previous pressed button
short CurrentButton = -1;

unsigned long currentMillis = 0;

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
  // BTN BOTTOM-LEFT
  if (U >= 510 && U <= 512) {
    return BTN_BOTTOM_LEFT;
  }
  // BTN BOTTOM-CENTRE
  else if (U >= 818 && U <= 819) {
    return BTN_BOTTOM_CENTRE;
  }
  // BTN BOTTOM-RIGHT
  else if (U >= 681 && U <= 682) {
    return BTN_BOTTOM_RIGHT;
  }
  // BTN RIGHT-BOTTOM
  else if (U >= 767 && U <= 768) {
    return BTN_RIGHT_BOTTOM;
  }
  // BTN RIGHT-TOP
  else if (U >= 852 && U <= 853) {
    return BTN_RIGHT_TOP;
  }
  // BTN LEFT-BOTTOM
  else if (U >= 806 && U <= 808) {
    return BTN_LEFT_BOTTOM;
  }
  // BTN LEFT-TOP
  else if (U <= 10) {
    return BTN_LEFT_TOP;
  }
  return BTN_NONE;
}

void resetTxtBtn() {
  if(CurrentChannel <= 0) {
    // Main help
    oledStr1 = "  HELLO WORLD!!!";
    oledStr2 = "Switch between chanel";
    oledStr3 = "with Cx button, x is";
    oledStr4 = "the activated channel.";
    oledStr5 = "Long press reinit";
    oledStr6 = "current channel.";
    oledStr7 = "Short press P1 for";
    oledStr8 = "switching between";
    oledStr9 = "pages. Long press switch";
    oledStr10 = "on/off screen.";
    oledStr11 = "    ------------  ";
  }else {
    oledStr1 = "";
    oledStr2 = "";
    oledStr3 = "";
    oledStr4 = "";
    oledStr5 = "";
    oledStr6 = "";
    oledStr7 = "";
    oledStr8 = "";
    oledStr9 = "";
    oledStr10 = "";
    oledStr11 = "";
  }
  
  TEXT_BOTTOM_LEFT = "";
  TEXT_BOTTOM_CENTER = "";
  TEXT_BOTTOM_RIGHT = "";
  TEXT_RIGHT_BOTTOM = "";
  TEXT_RIGHT_TOP = "";
}

// Transmit btn pressed
void transmitSignal(short btn) {
  char charBuf[50];
  String s = String(CurrentChannel) + "" + String(btn);
  s.toCharArray(charBuf, 50);
  vw_send((uint8_t *)charBuf, strlen(charBuf));
  vw_wait_tx();
}
void transmitSignal() {
  transmitSignal(CurrentButton);
  /*
  char charBuf[50];
  String s = String(CurrentChannel) + "" + String(CurrentButton);
  s.toCharArray(charBuf, 50);
  vw_send((uint8_t *)charBuf, strlen(charBuf));
  vw_wait_tx();
  */
}

void receiveSignal() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {
    short i;
    String action = "";
    String params = "";
    for (i = 0; i < buflen; i++) {
      // Channel code
      if(i == 0) {
        // not active module
        if(buf[i] != ChannelList[CurrentChannel]) {
          return;
        }
      }
      // Action code
      else if(i == 1){
        action = (String) char(buf[i]);
      }
      // Other params
      else {
        params += (String) char(buf[i]);
      }
    }
    manageSignal(action, params);
  }
}

void manageSignal(String action, String params) {
  char charBuf[params.length()+1];
  params.toCharArray(charBuf, (params.length()+1));

  //char dlm[] = "|";
  char *pch = strtok(charBuf, "|");
  short i = 0;
  while ( pch != NULL ) {
    // Init menu
    if(action == "m") {
      // Channel inited
      CurrentChannetInited = 1;
      if(i == 0) {
        // Module title
        oledStr1 = (String) pch;
      } else if(i == 1) {
        // Btn 1
        TEXT_BOTTOM_LEFT = (String) pch;
      } else if(i == 2) {
        // Btn 2
        TEXT_BOTTOM_CENTER = (String) pch;
      } else if(i == 3) {
        // Btn 3
        TEXT_BOTTOM_RIGHT = (String) pch;
      } else if(i == 4) {
        TEXT_RIGHT_BOTTOM = (String) pch;
      } else if(i == 5) {
        TEXT_RIGHT_TOP = (String) pch;
      }
    }
    // Acquisition
    else if(action == "a") {
      if(i == 0) {
        oledStr2 = (String) pch;
      } else if(i == 1) {
        oledStr3 = (String) pch;
      } else if(i == 2) {
        oledStr4 = (String) pch;
      } else if(i == 3) {
        oledStr5 = (String) pch;
      } else if(i == 4) {
        oledStr6 = (String) pch;
      }
    }
    pch = strtok (NULL, "|");
    i++;
  }

  //char *p, *i;
  //p = strtok_r(message,"|",&i);
  //oledStr1 = "-" + (String) atoi(message) + "-";
  
}

void reloadChannel(short prevBtn) {
  // Display page 1
  currentPage = 1;
  // Remove all button
  resetTxtBtn();
  // Loading
  if(CurrentChannel > 0) {
    oledStr1 = "Loading...";
  }
  CurrentChannetInited = 0;
  // Connecting...;
  // Transmit signal
  // Loop until module repond and channel initialized
  transmitSignal(prevBtn);
}

void setup() {
  resetTxtBtn();
  
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

void loop() {
  currentMillis++;
  
  // Skip si maintain btn
  if(CurrentButton == getBtn()) {
    // TODO : Btn maintained
  }
  else {
    short prevBtn = CurrentButton;
    CurrentButton = getBtn();
    short iInit = 0;
    // catch button
    switch(CurrentButton) {
      case BTN_BOTTOM_LEFT:
        currentMillis = 0;

        // Transmit signal
        transmitSignal();
        break;
      case BTN_BOTTOM_CENTRE:
        currentMillis = 0;
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_BOTTOM_RIGHT:
        currentMillis = 0;
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_RIGHT_BOTTOM:
        currentMillis = 0;
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_RIGHT_TOP:
        currentMillis = 0;
        
        // Transmit signal
        transmitSignal();
        break;
      case BTN_LEFT_TOP:
        currentMillis = 0;
        // Managed in BTN_NONE on release press
        break;
      case BTN_LEFT_BOTTOM:
        currentMillis = 0;
        // Managed in BTN_NONE on release press
        break;
      case BTN_NONE:
        // MANAGE HERE ONRELEASE AND PRESS DURATION
        // Short press
        if(currentMillis < 10) {
          switch(prevBtn) {
            case BTN_LEFT_TOP:
              // Swith channel
              CurrentChannel++;
              if(CurrentChannel >= sizeof(ChannelList)) CurrentChannel = 0;
              reloadChannel(prevBtn);
              break;
            case BTN_LEFT_BOTTOM:
              if(oled.getColorIndex() == 0) {
                oled.setColorIndex(1);
              }
              else if(oledStr7 == "" && oledStr8 == "" && oledStr9 == "" && oledStr10 == "" && oledStr11 == "") {
                // raf
              }else {
                currentPage++;
                if(currentPage > 2){
                  currentPage = 1;
                }
              }
              break;
            default:
              // Transmit signal
              transmitSignal();
              break;
          }
        }
        // long press
        else if(currentMillis >= 10) {
          switch(prevBtn) {
            case BTN_LEFT_TOP:
              // Reload channel without switching
              reloadChannel(prevBtn);
              break;
            case BTN_LEFT_BOTTOM:
              if(oled.getColorIndex() == 1) {
                oled.setColorIndex(0);
              }else {
                oled.setColorIndex(1);
              }
              break;
            default:
              // Transmit signal
              transmitSignal();
              break;
          }
        }
        currentMillis = 0;
      default:
        // TODO none
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
    //oledStr1.toUpperCase();
    oledDraw(oledStr1, 20, 10);

    // Page one
    if(currentPage == 1) {
      oledDraw(oledStr2, 15, 20);
      oledDraw(oledStr3, 15, 28);
      oledDraw(oledStr4, 15, 36);
      oledDraw(oledStr5, 15, 44);
      oledDraw(oledStr6, 15, 52);
    }
    // Page 2
    else if(currentPage == 2) {
      oledDraw(oledStr7, 15, 20);
      oledDraw(oledStr8, 15, 28);
      oledDraw(oledStr9, 15, 36);
      oledDraw(oledStr10, 15, 44);
      oledDraw(oledStr11, 15, 52);
    }
    // text for btn left
    oledDraw("C" + (String) CurrentChannel, 0, 10);
    oledDraw("P" + (String) currentPage, 0, 53);

    // text for btn bottom
    oledDraw(TEXT_BOTTOM_LEFT, 5, 63);
    oledDraw(TEXT_BOTTOM_CENTER, 57, 63);
    oledDraw(TEXT_BOTTOM_RIGHT, 105, 63);
    oledDraw(TEXT_RIGHT_BOTTOM, 114, 53);
    oledDraw(TEXT_RIGHT_TOP, 114, 10);

  } while ( oled.nextPage() );
}

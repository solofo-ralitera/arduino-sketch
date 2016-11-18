#include <VirtualWire.h>

// Btn code
#define BTN_BOTTOM_LEFT     '1'
#define BTN_BOTTOM_CENTRE   '2'
#define BTN_BOTTOM_RIGHT    '3'
#define BTN_RIGHT_BOTTOM    '4'
#define BTN_RIGHT_TOP       '5'
#define BTN_LEFT_TOP        '6'
#define BTN_NONE            '0'

#define MODULE_ID   '1'

#define PIN_RECEIVER  2
#define PIN_EMITTER   3

const String MODULE_TITLE = "MODULE 1";

String TransmitMessage = "";

char Message[VW_MAX_MESSAGE_LEN]; 
// Manage btn signale receive
void receiveSignal() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) {
    int i;
    for (i = 0; i < buflen; i++) {
      Message[i] = char(buf[i]);
      // Module code
      if(i == 0) {
        // not for me
        if(buf[i] != (byte) MODULE_ID) {
          return;
        }
      }
      // BTN code
      else if(i == 1){
        manageBtn(char(buf[i]));
      }
      // Other message
      else {
        // TODO
      }
    }
  }
}

void transmitSignal() {
  char charBuf[50];
  //char tString[24];
  //char hString[24];
  //char msg[27];
  //sprintf(msg, "%s, %s,", tString, hString);
  TransmitMessage = (String) MODULE_ID + "" + TransmitMessage;
  TransmitMessage.toCharArray(charBuf, (TransmitMessage.length()+1));
  vw_send((uint8_t *)charBuf, strlen(charBuf));
  vw_wait_tx();
}

void manageBtn(char btnCode) {
  switch(btnCode) {
      case BTN_BOTTOM_LEFT:
        // TODO
        break;
      case BTN_BOTTOM_CENTRE:
        // Acquisition
        TransmitMessage = "a|Status:|Temp:|Fan:";
        transmitSignal();        
        break;
      case BTN_BOTTOM_RIGHT:
        // TODO
        break;
      case BTN_RIGHT_BOTTOM:
        // TODO
        break;
      case BTN_RIGHT_TOP:
        // TODO
        break;
      case BTN_LEFT_TOP:
        // Active channel
        TransmitMessage = "m|"+MODULE_TITLE+"|...|ack|...|Sw2|Sw1";
        transmitSignal();
        break;
      case BTN_NONE:
      default:
        // TODO
        break;
    }
}

void setup()
{
  Serial.begin(9600);
    
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(PIN_RECEIVER);
  vw_set_tx_pin(PIN_EMITTER);
  vw_setup(4000);
  vw_rx_start();
}

void loop()
{
  receiveSignal();
}


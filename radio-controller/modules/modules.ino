/**************
 * Module 1
 * Data acuisition:
 *    - Fan speed
 *    - Temp with LM35
 * Switch
 *    - Power
 *    - Reset
 */

#include <VirtualWire.h>

// Btn code
#define BTN_BOTTOM_LEFT     '1'
#define BTN_BOTTOM_CENTRE   '2'
#define BTN_BOTTOM_RIGHT    '3'
#define BTN_RIGHT_BOTTOM    '4'
#define BTN_RIGHT_TOP       '5'
#define BTN_LEFT_TOP        '6'
#define BTN_LEFT_BOTTOM     '7'
#define BTN_NONE            '0'

#define MODULE_ID   '1'

#define PIN_RECEIVER  3
#define PIN_EMITTER   2

#define PIN_POWER_SW  4
#define PIN_RESET_SW  5

#define PIN_SECURITY_LAP  5000
unsigned long LAST_PIN_MILLIS = 0;

const String MODULE_TITLE = "SWITCH & DATA";

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
  char charBuf[TransmitMessage.length()+2];
  //char tString[24];
  //char hString[24];
  //char msg[27];
  //sprintf(msg, "%s, %s,", tString, hString);
  TransmitMessage = (String) MODULE_ID + "" + TransmitMessage;
  TransmitMessage.toCharArray(charBuf, (TransmitMessage.length()+1));
  vw_send((uint8_t *)charBuf, strlen(charBuf));
  vw_wait_tx();
}

void resetPins() {
    // Reset all switch pin
    digitalWrite(PIN_POWER_SW, LOW);
    digitalWrite(PIN_RESET_SW, LOW);  
}

void activePin(short pin) {
  digitalWrite(pin, HIGH);
  LAST_PIN_MILLIS = millis();
  Serial.println(LAST_PIN_MILLIS);
}

void manageBtn(char btnCode) {
  switch(btnCode) {
      case BTN_BOTTOM_LEFT:
        // Acquisition
        TransmitMessage = "a|Status:|Temp:|Fan:| | ";
        transmitSignal();        
        break;
      case BTN_BOTTOM_CENTRE:
        // Resel all pins, like BTN_NONE
        resetPins();
        break;
      case BTN_BOTTOM_RIGHT:
        // Help
        TransmitMessage = "a|acq:Data aquisition|cnl:Reset pins|Rst:Switch on|Pwr:Switch on| ";
        transmitSignal();        
        break;
      case BTN_RIGHT_BOTTOM:
        // RESET Switch
        activePin(PIN_RESET_SW);
        break;
      case BTN_RIGHT_TOP:
        // POWER Switch
        activePin(PIN_POWER_SW);
        break;
      case BTN_LEFT_TOP:
        // Active channel
        TransmitMessage = "m|"+MODULE_TITLE+"|acq|cnl| ? |Rst|Pwr";
        transmitSignal();
        break;
      case BTN_NONE:
        resetPins();
        break;
      default:
        // TODO
        break;
    }
}

void setup()
{
  //Serial.begin(9600);

  pinMode(PIN_POWER_SW, OUTPUT);
  pinMode(PIN_RESET_SW, OUTPUT);
  resetPins();
  
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(PIN_RECEIVER);
  vw_set_tx_pin(PIN_EMITTER);
  vw_setup(4000);
  vw_rx_start();
}

void loop()
{
  //Reset pin for security if activated for PIN_SECURITY_LAP time
  if(millis() - LAST_PIN_MILLIS > PIN_SECURITY_LAP) {
    resetPins();
  }
  receiveSignal();
}


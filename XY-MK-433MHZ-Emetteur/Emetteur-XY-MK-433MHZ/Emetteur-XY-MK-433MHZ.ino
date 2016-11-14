//simple Tx on pin D12
//Written By : Mohannad Rawashdeh
// 3:00pm , 13/6/2013
//http://www.genotronex.com/
// http://www.instructables.com/id/RF-315433-MHz-Transmitter-receiver-Module-and-Ardu/step3/Arduino-Virtual-Wire-Library/
//..................................

#include <VirtualWire.h>

const int TransmitterPin = 12;
const int PotarPin = A0;
int ValueToSend;

char Message[VW_MAX_MESSAGE_LEN]; 

void setup() {
  vw_set_ptt_inverted(true); //
  vw_set_tx_pin(TransmitterPin);
  vw_setup(4000);// speed of data transfer Kbps
}

void loop(){
  ValueToSend = analogRead(PotarPin);

  // convertion de l'int en tableau de char 
  itoa(ValueToSend, Message, 10);  // 10 car décimal
  vw_send((uint8_t *)Message, strlen(Message));
  vw_wait_tx(); // Wait until the whole message is gone

  delay(200);
}

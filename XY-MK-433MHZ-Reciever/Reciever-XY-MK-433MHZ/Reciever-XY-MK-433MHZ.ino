//simple Tx on pin D12
//Written By : Mohannad Rawashdeh
// 3:00pm , 13/6/2013
//http://www.genotronex.com/
//..................................
#include <VirtualWire.h>

const int RecieverPin = 12;

int Nombre;
char Message[VW_MAX_MESSAGE_LEN]; 

void setup()
{
  Serial.begin(9600);
    
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(RecieverPin);
  vw_setup(4000);  // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  if (vw_get_message(buf, &buflen)) {
    int i;

    for (i = 0; i < buflen; i++) {               
      Message[i] = char(buf[i]);
    }

    Message[buflen] = '\0';

    // Conversion du tableau de chars en int:
    String s = Message;
    
    Serial.println(s);
  }
}

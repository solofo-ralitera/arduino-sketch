#include <Wire.h>

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

void setup() {
  Wire.begin();
  Serial.begin(9600);

  //eeprom_i2c_write(B01010000, 0, 9);
  //eeprom_i2c_write(B01010000, 1, 2);
  
  
  for(int i = 0; i < 100; i++) {
    eeprom_i2c_write(B01010000, i, i);
    delay(100);
  }
  
  Serial.println("Writen to memory!");

}

void loop() {
  for(int i = 0; i < 100; i++) {
    byte r = eeprom_i2c_read(B01010000, i);
    Serial.print(i);
    Serial.print(" - ");
    Serial.print(r);
    Serial.print("\n");
    delay(1000);
  }
}


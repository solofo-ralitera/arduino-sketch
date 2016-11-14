// Include library found at 
// https://docs.google.com/file/d/0Bwrp4uluZCpNdE9oWTY0M3BncTA/edit 
#include "SevSeg.h"
#include <OneWire.h> // Inclusion de la librairie OneWire
#include <SPI.h>     // Inclusion de la librairie SPI (communications bas niveau)
#include <SD.h>      // Inclusion de la librairie SD (carte SD en format FAT)

#define BROCHE_ONEWIRE 7 // Broche utilisée pour le bus 1-Wire
#define DS18B20 0x28     // Adresse 1-Wire du DS18B20

//Create an instance of the object.
SevSeg sevseg;
unsigned long timer;

int temp = 0;
bool relais = false;

OneWire ds(BROCHE_ONEWIRE); // Création de l'objet OneWire ds

void setup() {
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  
  timer = millis();
  
  // Set up pins
  // Arguments : 
  // First 0 is for common cathode
  // Following 0 to 11 numbers are for Arduino pins
  // connected to display in the following order
  // 1,2,3,4,A,B,C,D,E,F,G,DP
  sevseg.Begin(0,0,1,2,3,4,5,6,7,8,9,10,11);
}
 
void loop() {   
  //Check if 10ms has elapsed
  unsigned long mils = millis();
  if (mils-timer >= 1000) {
    timer = mils;
    //float ftemp = analogRead(A0)*5./1023*100;
    //temp = (int) ftemp;
    //temp = getTemperature();
    //Serial.println(temp);
    temp++;
    
    if(relais) {
      relais = false;
      digitalWrite(13, HIGH);
    }else {
      relais = true;
      digitalWrite(13, LOW);
    }
  }
  
  // Prepare number to display, and dot position 
  // (0 to 3, other number means no dot)  
  sevseg.NewNum(temp, 0);
  sevseg.PrintOutput();  
  
}

int getTemperature(){
  int temp = analogRead(A0)*5./1023*100;
  return temp;
}

float getTemperatureWire(){
  byte data[9], addr[8];
  // data : Données lues depuis le scratchpad
  // addr : adresse du module 1-Wire détecté
 
  if (!ds.search(addr)) { // Recherche un module 1-Wire
    ds.reset_search();    // Réinitialise la recherche de module
    return false;         // Retourne une erreur
  }
 
  if (OneWire::crc8(addr, 7) != addr[7]) // Vérifie que l'adresse a été correctement reçue
    return false;                        // Si le message est corrompu on retourne une erreur
 
  if (addr[0] != DS18B20) // Vérifie qu'il s'agit bien d'un DS18B20
      return false;         // Si ce n'est pas le cas on retourne une erreur
 
  ds.reset();             // On reset le bus 1-Wire
  ds.select(addr);        // On sélectionne le DS18B20
 
  ds.write(0x44, 1);      // On lance une prise de mesure de température
  delay(800);             // Et on attend la fin de la mesure
 
  ds.reset();             // On reset le bus 1-Wire
  ds.select(addr);        // On sélectionne le DS18B20
  ds.write(0xBE);         // On envoie une demande de lecture du scratchpad
 
  for (byte i = 0; i < 9; i++) // On lit le scratchpad
    data[i] = ds.read();       // Et on stock les octets reçus
 
  // Calcul de la température en degré Celsius
  float temp = ((data[1] << 8) | data[0]) * 0.0625; 
 
  // Pas d'erreur
  return temp;
}

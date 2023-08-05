#include <SoftwareSerial.h>

#define RXD_PIN 2
#define TXD_PIN 3

SoftwareSerial bt(RXD_PIN, TXD_PIN);

char data;

void setup()
{ 
  Serial.begin(9600); 
  bt.begin(9600);
  Serial.println("Ready......");
}
void loop() { 
  Serial.flush(); 
  Serial.print("CMD : ");
  while(!Serial.available());
  while(Serial.available()) { 
    data = Serial.read(); 
    if(data == -1) 
      break;
    bt.print(data); 
    Serial.print(data); 
    delay(1); 
  } 
  Serial.println();
  delay(1000); 
  Serial.print("Return : ");
  while(bt.available()) { 
    data = bt.read(); 
    if(data == -1) 
      break;
    Serial.print(data); 
    delay(1); 
  } 
  Serial.print("\n\n");
}
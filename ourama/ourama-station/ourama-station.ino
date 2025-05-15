/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(108);
  radio.setRetries(5,15);
  radio.startListening();
}

void loop() {

  char receivedUID[9]={0};
  if (radio.available())  {
    radio.read(&receivedUID, sizeof(receivedUID));
    // Serial.print("Received UID:");
    Serial.println(receivedUID);
    radio.flush_rx();
  }
}
//Arduino Code - RC522 Read RFID Tag UID
 
#include <SPI.h>
#include <MFRC522.h>
#include <nRF24L01.h>
#include <RF24.h>

 
unsigned long previousTime=0; // millis is unsigned long var type

// RFID reader
#define SS_PIN 10
#define RST_PIN 7
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

// Radio module 
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
 
void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init RC522 

  // Radio module
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
 
void loop() {
    // Start tumer
    unsigned long currentTime = millis();
    
    if (currentTime - previousTime <= 500){
        // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
        if ( ! rfid.PICC_IsNewCardPresent())
        return;

        // Verify if the NUID has been readed
        if ( ! rfid.PICC_ReadCardSerial())
        return;

        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

        Serial.print(F("RFID Tag UID:"));
        printHex(rfid.uid.uidByte, rfid.uid.size);
        Serial.println("");

        rfid.PICC_HaltA(); // Halt PICC
    } else if (currentTime - previousTime <= 2000 && currentTime - previousTime > 500) {
        const char text[] = "I'm inside your walls";
        radio.write(&text, sizeof(text));
        delay(500);
        previousTime = currentTime;
    }

}
 
//Routine to dump a byte array as hex values to Serial. 
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
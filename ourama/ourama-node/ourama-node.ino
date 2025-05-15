#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <nRF24L01.h>
#include <RF24.h>

MFRC522DriverPinSimple ss_pin(5);

MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
MFRC522 mfrc522{driver};         // Create MFRC522 instance

// HSPI instance
SPIClass * hspi = new SPIClass(HSPI);

// nRF24L01 control pins
#define CE_PIN   4
#define CSN_PIN 15

RF24 radio(CE_PIN, CSN_PIN);  // CE, CSN

const byte address[6]="00001";

void setup() {
  Serial.begin(9600);
  while(!Serial);

  mfrc522.PCD_Init();
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  Serial.println(F("Scan PICC to see UID"));

  hspi->begin(14, 12, 13, 15);
  radio.begin(hspi);
  if (!radio.begin()) {
    Serial.println("nRF24L01 not responding!");
    while (1);
  }
  radio.openWritingPipe(address);
  radio.setChannel(108);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setRetries(5, 15);  // 5 retries, 15 delay
  radio.stopListening(); 
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards.
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}

  Serial.print("Card UID: ");
  MFRC522Debug::PrintUID(Serial, (mfrc522.uid));
  Serial.println();

  // Save the UID on a String variable
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uidString += "0"; 
    }
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println(uidString);
  
  mfrc522.PICC_HaltA(); // Halt PICC

  delay(500);
  
  char uidBuffer[9] = {0};
  uidString.toCharArray(uidBuffer, sizeof(uidBuffer));
  bool success = radio.write(&uidBuffer, sizeof(uidBuffer));
  if (success) {
      Serial.println("nRF24L01: Transmission successful!");
   }  else {
      Serial.println("nRF24L01: Transmission failed!");
      radio.printDetails();  // Only print on fail to avoid clutter
   }
   delay(1000);
}

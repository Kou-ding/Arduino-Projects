/*
  Temperature and Humidity sensor DHT11

  Read the temperature and humidity from the DHT11 and provide feedback through the LED.

*/

#include <DHT.h>

#define DHTPIN 2     // Pin where the DHT11 is connected
#define LEDPIN 3     // Pin where the led is connected
#define DHTTYPE DHT11   // Define the sensor type

DHT dht(DHTPIN, DHTTYPE);
unsigned long previousTime=0; // millis is unsigned long var type
const long printInterval = 2000; // keep printInterval constant throughout the program

void setup() {
  // Initialize the dht sensor
  dht.begin();
  // Begin serial communication at port 9600
  Serial.begin(9600);
  // Initialize the LEDPIN as output.
  pinMode(LEDPIN, OUTPUT);
}

void blinker(int ledDelay){
  digitalWrite(LEDPIN, HIGH);  // turn the LED on (HIGH voltage)
  delay(ledDelay/2);           // wait
  digitalWrite(LEDPIN, LOW);   // turn the LED off (LOW voltage)
  delay(ledDelay/2);           // wait 
}

void loop() {
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  unsigned long currentTime = millis();
  if (currentTime - previousTime >= printInterval){
    previousTime = currentTime;
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
  }
  
  // LED temperature feedback
  int ledDelay;
  if (t>=20 && t<=30){
    digitalWrite(LEDPIN, LOW);
  }else if (t>30){
    ledDelay = 2000;
    blinker(ledDelay);
  }else if (t<20){
    ledDelay = 1000;
    blinker(ledDelay);
  }
}

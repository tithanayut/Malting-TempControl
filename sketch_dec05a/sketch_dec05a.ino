#include "DHT.h"

#define DHTPIN 6
#define DHTTYPE DHT11 // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  dht.begin();
}

void loop() {
  float t = dht.readTemperature();
  
  if (isnan(t)){
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
  }
}

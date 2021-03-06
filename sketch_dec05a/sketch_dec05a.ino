#include "DHT.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"

#define DHTPIN 2
#define DHTTYPE DHT11 // DHT 11 

const char* ssid = "SSID"; //WiFi SSID
const char* password = "PASSWORD"; //WiFi Password

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(D1, OUTPUT);
  
  Serial.begin(9600); 
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
}

boolean state = false;

void loop() {
  int t = dht.readTemperature();
  int h = dht.readHumidity();
  
  if (isnan(t)){
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");

    //REPORT
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String apicall = "http://SERVERADDRESS/malting/api.php?temp=" + String(t) + "&humid=" + String(h); //API CALL ADDRESS
      Serial.println(apicall);
      http.begin(apicall);
      int httpCode = http.GET();   
 
      if (httpCode > 0) { 
        String payload = http.getString();
        Serial.print("HTTP API Reply: "); 
        Serial.println(payload);                  
      }
      http.end();
    }


    //RELAY
    if(state == false){
      if(t >= 19){
        digitalWrite(D1, HIGH);
      }else{
        digitalWrite(D1, LOW);
        state = true;
      }
    }
    if(state == true){
      if(t >= 21){
        digitalWrite(D1, HIGH);
        state = false;
      }else{
        digitalWrite(D1, LOW);
      }
    }
      
    delay(10000);
  }
}

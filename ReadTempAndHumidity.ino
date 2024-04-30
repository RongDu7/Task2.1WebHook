/**
 * DHT11 Sensor Reader
 * This sketch reads temperature and humidity data from the DHT11 sensor and prints the values to the serial port.
 * It also handles potential error states that might occur during reading.
 *
 * Author: Dhruba Saha
 * Version: 2.1.0
 * License: MIT
 */

// Include the DHT11 library for interfacing with the sensor.
#include <DHT11.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <SPI.h>

// Create an instance of the DHT11 class.
// - For Arduino: Connect the sensor to Digital I/O Pin 2.
// - For ESP32: Connect the sensor to pin GPIO2 or P2.
// - For ESP8266: Connect the sensor to GPIO2 or D4.
#define DHTPIN 3
#define DHTTYPE DHT11

DHT11 dht11(DHTPIN);
WiFiClient client;

char ssid[] = "_zsEXPRESS";
char pass[] = "zGd7699!";                 


unsigned long myChannelNumber = 2516564;
const char * myWriteAPIKey = "KYEPHOK2MB61WBH6";

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, pass);
    ThingSpeak.begin(client); 

    while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
    
}

void loop() {

  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  if (result == 0) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\tHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");


    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);

    int alarmStatus = (temperature < 24 || temperature > 27) ? 2 : 1;
    ThingSpeak.setField(3, alarmStatus);


    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (alarmStatus == 2) {
      Serial.println("Alarm triggered!");
    }
  } else {
    Serial.println(DHT11::getErrorString(result));
  }

    
  delay(1500);
}



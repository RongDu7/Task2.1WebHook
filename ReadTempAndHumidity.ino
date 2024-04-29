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
const char * myWriteAPIKey = "Q5U4CIGTS6D1M6L8";

void setup() {
    // Initialize serial communication to allow debugging and data readout.
    // Using a baud rate of 9600 bps.
    Serial.begin(9600);
    WiFi.begin(ssid, pass);
    ThingSpeak.begin(client);  // Initialize ThingSpeak

    while (WiFi.status() != WL_CONNECTED) {  // Wait for the connection to the network
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
    
    // Uncomment the line below to set a custom delay between sensor readings (in milliseconds).
    // dht11.setDelay(500); // Set this to the desired delay. Default is 500ms.
}

void loop() {

  int temperature = 0;
  int humidity = 0;

  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int result = dht11.readTemperatureHumidity(temperature, humidity);

  // Check the results of the readings.
  // If the reading is successful, print the temperature and humidity values.
  // If there are errors, print the appropriate error messages.
  if (result == 0) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C\tHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    } else {
      // Print error message based on the error code.
      Serial.println(DHT11::getErrorString(result));
    }

    // Set the fields with the values
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  // Write to the ThingSpeak channel
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // ThingSpeak updates rate is at least 15 seconds for a free account.
  delay(2000);
}

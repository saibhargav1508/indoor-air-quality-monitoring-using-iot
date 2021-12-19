#include "ThingSpeak.h"

unsigned long myChannelNumber = 000; // replace 0000000 with your channel ID
const char * myWriteAPIKey = "XYZ"; // replace XYZ with your channel write API Key

#include <ESP8266WiFi.h>

char ssid[] = "***";   // your network SSID (name)
char pass[] = "***";   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

void setup() {
  Serial.begin(115200);
  delay(100);
 // WiFi.mode(WIFI_STA);

    // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  ThingSpeak.begin(client);
}

void loop() {
  float h = analogRead(A0);
  if (isnan(h))
  {
    Serial.println("Failed to read from MQ-5 sensor!");
    return;
  }
  
    // Measure Signal Strength (RSSI) of Wi-Fi connection
  int i=0;
  for(i=0;i<3;i++)
  {
    // Write value to Field 1 of a ThingSpeak Channel
    int httpCode = ThingSpeak.writeField(myChannelNumber, 1, String(h/1023*100), myWriteAPIKey);
  
    if (httpCode == 200) {
      Serial.print("Gas Level: ");
      Serial.println(h/1023*100);
      Serial.println("Channel write successful.");
    }
    else {
      Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
    }
  
    // Wait 10 seconds to update the channel again
    delay(20000);
  }
  // puts the nodeMCU to deepsleep for 20 seconds
  ESP.deepSleep(20e6);
}

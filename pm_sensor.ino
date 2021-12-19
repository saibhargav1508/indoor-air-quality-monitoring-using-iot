#include "ESP8266WiFi.h"
#include "ThingSpeak.h"

int pin = 14;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000;//sampe 1s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

// WiFi parameters to be configured
const char* ssid = "***"; // Write here your router's username
const char* password = "***"; // Write here your router's passward

#define SECRET_CH_ID 000     // replace 0000000 with your channel ID
#define SECRET_WRITE_APIKEY "XYZ"   // replace XYZ with your channel write API Key

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
WiFiClient client;
int keyIndex = 0;

void setup() {
  Serial.begin(115200);
  pinMode(14,INPUT);
  starttime = millis();//get the current time;

  //WiFi.mode(WIFI_STA);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED){
     WiFi.begin(ssid, password);
     delay(5000);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");

  ThingSpeak.begin(client);
}

void loop() {
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  for(int i = 0; i<3; i++)
  {
    if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
    {
      ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
      concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
      Serial.print(ratio);
      Serial.print("concentration = ");
      Serial.print(concentration);
      Serial.print(" pcs/0.01cf  -  ");
      if (concentration < 1.0) {
       Serial.println("It's a smokeless and dustless environment"); 
    }
      if (concentration > 1.0 && concentration < 20000) {
       Serial.println("It's probably only you blowing air to the sensor :)"); 
      }
      
      if (concentration > 20000 && concentration < 315000) {
       Serial.println("Smokes from matches detected!"); 
      }
        if (concentration > 315000) {
       Serial.println("Smokes from cigarettes detected! Or It might be a huge fire! Beware!"); 
    }
      
      lowpulseoccupancy = 0;
      starttime = millis();
    }
    Serial.println("Trying to write data to the cloud");
    Serial.println(concentration);
    // Write value to Field 1 of a ThingSpeak Channel
    int httpCode = ThingSpeak.writeField(myChannelNumber, 1, String(concentration), myWriteAPIKey);
  
    if (httpCode == 200) {
        Serial.println("Channel write successful.");
      }
    else {
        Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
    }
    delay(20000);
  }
  // puts the nodeMCU to deepsleep for 20 seconds
  ESP.deepSleep(20e6);
}

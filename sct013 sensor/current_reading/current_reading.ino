#include "EmonLib.h"
#include "ThingSpeak.h"
#include <WiFi.h>                  // Include Emon Library
EnergyMonitor emon1;   
#define uS_TO_S_FACTOR 1000000ULL 
#define TIME_TO_SLEEP  10 

WiFiClient  client;     
const char* ssid = "RKnet";
const char* password = "xmn3dw7v2qquzf5";

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "BC4PMZETG7HZVEXT";

const int LED_PIN = 5;
           

void setup()
{  
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);
  emon1.current(13, 111.1);             // Current: input pin, calibration.
  
}

void loop()
{
  digitalWrite(LED_PIN,255);
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  
  Serial.print(Irms*230.0);	       // Apparent power
  Serial.print(" ");
  Serial.println(Irms);		       // Irms
}
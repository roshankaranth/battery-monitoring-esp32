#include "ThingSpeak.h"
#include <WiFi.h>   

#define uS_TO_S_FACTOR 1000000ULL 
#define TIME_TO_SLEEP  10 

WiFiClient  client;     
const char* ssid = "RKnet";
const char* password = "xmn3dw7v2qquzf5";
unsigned long myChannelNumber = 2;
const char* myWriteAPIKey = "JAEMML0I9GFOKKGP";

const int LED_PIN = 5;
const int sensorIn = 34;      
int mVperAmp = 185;           
int Watt = 0;
float Voltage = 0;
float VRMS = 0;
float AmpsRMS = 0;
 
 
void setup() {
  Serial.begin (115200); 
  pinMode(LED_PIN,OUTPUT);
  delay(1000);
  digitalWrite(5,255);

  //Connect to WiFi
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  
  if(WiFi.status()!=WL_CONNECTED){
    Serial.print("Attempting to connect");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);
      delay(5000);
    }
    Serial.println("\nConnected");
  }

  Serial.println (""); 
  Voltage = getVPP();
  VRMS = (Voltage/2.0) *0.707;   //root 2 is 0.707
  AmpsRMS = ((VRMS * 1000)/mVperAmp)-0.3; //0.3 is the error I got for my sensor
  Serial.print(AmpsRMS);
  Serial.print(" Amps RMS  ---  ");
  Watt = (AmpsRMS*240/1.2);
  Serial.print(Watt);
  Serial.println(" Watts");
  delay(100);

  ThingSpeak.setField(1,AmpsRMS);
  ThingSpeak.setField(2,Watt);


  int x = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);

  if(x==200){
    Serial.println("Channel update Succesfull!");
  }else{
    Serial.println("Problem updating channel. HTTP error code : " + String(x));
  }

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");


  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();

}
 
void loop() {
  Serial.println (""); 
  Voltage = getVPP();
  VRMS = (Voltage/2.0) *0.707;  
  AmpsRMS = ((VRMS * 1000)/mVperAmp)-0.3;
 
  Serial.print(AmpsRMS);
  Serial.print(" Amps RMS  ---  ");
  Watt = (AmpsRMS*240/1.2);

  Serial.print(Watt);
  Serial.println(" Watts");
delay (100);
}
 
float getVPP()
{
  float result;
  int readValue;                
  int maxValue = 0;             
  int minValue = 4096;          
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000)
   {
       readValue = analogRead(sensorIn);
       if (readValue > maxValue) 
       {
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           minValue = readValue;
       }
   }
   
   result = ((maxValue - minValue) * 3.3)/4096.0; 
      
   return result;
 }
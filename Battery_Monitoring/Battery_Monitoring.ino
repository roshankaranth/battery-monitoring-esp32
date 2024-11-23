#include "ThingSpeak.h"
#include <Adafruit_INA219.h>
#include <WiFi.h>
#define uS_TO_S_FACTOR 1000000ULL 
#define TIME_TO_SLEEP  10        

Adafruit_INA219 ina219;
WiFiClient  client;

const char* ssid = "RKnet";
const char* password = "xmn3dw7v2qquzf5";

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "BC4PMZETG7HZVEXT";

const int LED_PIN = 5;


void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
  delay(1000);  

  float  current = 0.0;
  float bus_voltage = 0.0;
  float  load_voltage = 0.0;
  float  shunt_voltage= 0.0;
  float  power= 0.0;



  while (! ina219.begin()) {
    digitalWrite(LED_PIN,HIGH);
    delay(1000);
    digitalWrite(LED_PIN,LOW);
    delay(1000);
    Serial.println("Failed to find INA219 chip");
    delay(1000);
    
  }
  
  shunt_voltage = ina219.getShuntVoltage_mV();
  bus_voltage = ina219.getBusVoltage_V();
  current = ina219.getCurrent_mA();
  power = ina219.getPower_mW();
  load_voltage = bus_voltage + (shunt_voltage / 1000);
  

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

    ThingSpeak.setField(1,shunt_voltage);
    ThingSpeak.setField(2,bus_voltage);
    ThingSpeak.setField(3,current);
    ThingSpeak.setField(4,power);
    ThingSpeak.setField(5,load_voltage);


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
 
}
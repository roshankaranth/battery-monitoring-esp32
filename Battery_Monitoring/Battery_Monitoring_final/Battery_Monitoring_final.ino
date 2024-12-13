#include "ThingSpeak.h"
#include <Adafruit_INA219.h>
#include <WiFi.h>
#include "time.h"
#define uS_TO_S_FACTOR 1000000ULL 
#define TIME_TO_SLEEP  10   

Adafruit_INA219 ina219;
WiFiClient  client;

RTC_DATA_ATTR int cycle_count = 0;
RTC_DATA_ATTR float shunt_voltage[6];
RTC_DATA_ATTR float bus_voltage[6];
RTC_DATA_ATTR float current[6];
RTC_DATA_ATTR float power[6];
RTC_DATA_ATTR float load_voltage[6];
RTC_DATA_ATTR long timestamp[6];
RTC_DATA_ATTR long start_time;

const int LED_PIN = 18;

void setup() {
  
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
  delay(1000); 
  if(cycle_count==0) getTimeStamp();
  WiFi.disconnect(true);
  btStop();
  WiFi.mode(WIFI_OFF);   
  Serial.println("Modem Sleep mode enabled.");
  delay(1000);
}

void loop() {
  
  measureCurrent();
  delay(1000);
  if(cycle_count==3) sendDataToCloud();
  delay(1000);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Entering Deep Sleep...");
  esp_deep_sleep_start();
  Serial.println("Woke up from Deep Sleep!");

}

//Send data to thingspeak
void sendDataToCloud() {

  
  const char* ssid = "RKnet";
  const char* password = "xmn3dw7v2qquzf5";
  unsigned long myChannelNumber = 1;
  const char* myWriteAPIKey = "BC4PMZETG7HZVEXT";
  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  if(WiFi.status()!=WL_CONNECTED){
    Serial.print("Attempting to connect");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);
      delay(1000);
    }
    Serial.println("\nConnected");
  }
  delay(1000);
  for(int i = 0 ; i <= 6 ; i++){

        ThingSpeak.setField(1,shunt_voltage[i]);
        ThingSpeak.setField(2,bus_voltage[i]);
        ThingSpeak.setField(3,current[i]);
        ThingSpeak.setField(4,power[i]);
        ThingSpeak.setField(5,load_voltage[i]);
        ThingSpeak.setField(6,timestamp[i]);


      int x = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);

      if(x==200){
        Serial.println("Channel update Succesfull!");
      }else{
        Serial.println("Problem updating channel. HTTP error code : " + String(x));
      }
      delay(15000);
  }


  cycle_count = 0;
 
}

//Fetch sensor data
void measureCurrent() {
  delay(1000);
  timestamp[cycle_count] = start_time;
  start_time += 20;
  while (!ina219.begin()) {
    digitalWrite(LED_PIN,HIGH);
    delay(1000);
    digitalWrite(LED_PIN,LOW);
    delay(1000);
    Serial.println("Failed to find INA219 chip");
    delay(1000);
    
  }

  Serial.println("INA219 sensor detected. Fetching data...");
  
  shunt_voltage[cycle_count] = ina219.getShuntVoltage_mV();
  bus_voltage[cycle_count] = ina219.getBusVoltage_V();
  current[cycle_count] = ina219.getCurrent_mA();
  power[cycle_count] = ina219.getPower_mW();
  load_voltage[cycle_count] = bus_voltage[cycle_count] + (shunt_voltage[cycle_count] / 1000);
  Serial.printf("Measurement %d: Shunt Voltage = %.2f mV, Bus Voltage = %.2f V, Current = %.2f mA, Power = %.2f mW, Load Voltage = %.2f V\n",
                cycle_count, shunt_voltage[cycle_count], bus_voltage[cycle_count],
                current[cycle_count], power[cycle_count], load_voltage[cycle_count]);
  cycle_count++;
  


}

void getTimeStamp(){

  const char* ntpServer = "pool.ntp.org";
  const long  gmtOffset_sec = 0;
  const int   daylightOffset_sec = 3600;
  unsigned long epochTime; 
  const char* ssid = "RKnet";
  const char* password = "xmn3dw7v2qquzf5";
  WiFi.mode(WIFI_STA);

  if(WiFi.status()!=WL_CONNECTED){
    Serial.print("Attempting to connect");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);
      delay(1000);
    }
    Serial.println("\nConnected");
  }
  delay(1000);

  configTime(0, 0, ntpServer);

  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return ;
  }
  time(&now);
  start_time = now;

}


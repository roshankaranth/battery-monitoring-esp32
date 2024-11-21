#include "ThingSpeak.h"
#include <Adafruit_INA219.h>
#include <WiFi.h>
#define uS_TO_S_FACTOR 1000000ULL 
#define TIME_TO_SLEEP  10        

Adafruit_INA219 ina219;
WiFiClient  client;

//RTC_DATA_ATTR int bootCount = 0;
const char* ssid = "RKnet";
const char* password = "xmn3dw7v2qquzf5";

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "BC4PMZETG7HZVEXT";


void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_Setup cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); 
    case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
    default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void voltage_to_chargeCapacity(float voltage, float current){
    
    float[] pcoeff_0.2C = {-5.25065116e-17,  2.40031902e-14, -4.64422175e-12,  4.94969488e-10, -3.16731172e-08,  1.24344719e-06, -2.93492009e-05,  3.90554976e-04,-2.47430678e-03, -6.27397332e-03,  4.13122888e+00};
    float[] pcoeff_0.5C = {-3.58706976e-17,  1.55898972e-14, -2.82266036e-12,  2.74347220e-10, -1.52871361e-08,  4.73978642e-07, -6.60775629e-06, -1.83370332e-05, 1.69918269e-03, -2.72081171e-02,  4.09033666e+00};
    float[] pcoeff_1.0C = {-1.47427123e-17,  5.77398139e-15, -8.86523273e-13,  6.25159244e-11, -1.16810077e-09, -1.20838481e-07,  9.39945936e-06, -2.92684192e-04, 4.63391240e-03, -4.47995075e-02,  4.01924810e+00};
    float[] pcoeff_2.0C = {1.34677655e-17, -7.40215873e-15,  1.72958855e-12, -2.25998142e-10, 1.82369202e-08, -9.45908591e-07,  3.17718017e-05, -6.77976193e-04, 8.78290729e-03, -6.97271581e-02,  3.87939048e+00};
    float[] pcoeff_3.0C = {9.10312201e-17, -4.06336090e-14, 7.79442723e-12, -8.41323281e-10, 5.62463839e-08, -2.42000816e-06,  6.76407020e-05, -1.21618725e-03, 1.36777701e-02, -9.53165125e-02,  3.74781486e+00};

    float discarge_rate = current/2.6;

}


void setup() {
  Serial.begin(115200);
  delay(1000);  

  float  current = 0.0;
  float bus_voltage = 0.0;
  float  load_voltage = 0.0;
  float  shunt_voltage= 0.0;
  float  power= 0.0;

  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  shunt_voltage = ina219.getShuntVoltage_mV();
  bus_voltage = ina219.getBusVoltage_V();
  current = ina219.getCurrent_mA();
  power = ina219.getPower_mW();
  load_voltage = bus_voltage + (shunt_voltage / 1000);
  
  print_wakeup_reason();

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

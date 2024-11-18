Device name  : ESP WROOM 32
Board name : ESP32-WROOM-DA-Module
Port : TTYUSB0

Code to connect to WiFi : 

```C
#include <WiFi.h>

const char* ssid = "RKnet";
const char* password = "xmn3dw7v2qquzf5";

void setup(){

Serial.begin(115200);
WiFi.begin(ssid,password);

while(WiFi.status() != WL_CONNECTED){
delay(500);
Serial.println("Connecting to WiFi...");
}

Serial.println("Connected to WiFi network");
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address : ");
Serial.println(WiFi.localIP());
}

void loop(){

}
```

Operating conditions:

 - Power Supply voltage : 3.0V - 3.6V (typical : 3.3V)
 - Current delivered by external power supply : 0.5

### References

- https://randomnerdtutorials.com/power-esp32-esp8266-solar-panels-battery-level-monitoring/
- https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.instructables.com/Solar-Power-for-ArduinoESP32/&ved=2ahUKEwj57NP_t8KJAxX0sFYBHVJYMFwQFnoECC0QAQ&usg=AOvVaw3CE08A2hjHGiTl1z0ZRuXp
- https://blog.voltaicsystems.com/solar-powered-esp32-over-wifi-application-tutorial/

### Components required for power conversion:

- Solar Panels
	- Output voltage of 5-6 V. To charge battery faster, connect more solar panels in parallel.
- TP4056 battery charger module.
	- Circuit Protection, prevents battery over-voltage and reverse polarity connection.
- Lithium Batteries.
	- Output 4.2 V when fully charged.
- MCP1700-3302E LDO (HT7333-A)
	- linear voltage regulator v/s LDO
	- 100nF ceramic capacitor and 100uF electrolytic capacitor
### Components for Battery voltage level monitoring

- voltage divider
	- 27K ohm and 100k ohm resistor
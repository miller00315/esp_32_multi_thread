#include <WiFi.h>
#include <WiFiMulti.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_ipc.h>

#define wifi_ssid "CLARO_2G24445D"
#define wifi_password "AF24445D"
#define USE_SERIAL Serial
#define LED_BUILTIN 2

WiFiMulti wifiMulti;

void sensorLoop(void *arg) {
  Serial.print("This loop runs on PRO_CPU which id is: ");
  Serial.println(xPortGetCoreID());
    
  while(true) {
    int touchInputValue = touchRead(15);

    delay(10);
    
    if(touchInputValue < 20) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void wifiMonitor(void * arg) {
  Serial.print("This loop runs on PRO_CPU which id is:");
  Serial.println(xPortGetCoreID());
  
  while(true) {
    if((wifiMulti.run() == WL_CONNECTED)) {
      delay(1000);
      Serial.println("connected");
      delay(1000);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
      
      delay(50);

      digitalWrite(LED_BUILTIN, LOW);
      
      delay(50);
    }
  }
}


TaskHandle_t SensortLoopTask, WifiMonitorTask;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  USE_SERIAL.begin(115200);
  
  wifiMulti.addAP(wifi_ssid, wifi_password);

  xTaskCreatePinnedToCore(sensorLoop, "SensorLoop", 5000, NULL, 2, &SensortLoopTask,    0);
  delay(500);
  xTaskCreatePinnedToCore(wifiMonitor, "WifiMonitor", 5000, NULL, 2, &WifiMonitorTask, 1);
}

void loop() {}

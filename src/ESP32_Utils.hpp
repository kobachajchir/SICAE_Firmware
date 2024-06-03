#pragma once
#include <Globals.h>

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED && (millis() - connectionTimeCounter) >= WIFI_WAIT_TIME) {
    APMODE = 1;
    CONN_ESTABLISHED = 1;
    WiFi.mode(WIFI_AP);
   while(!WiFi.softAP(AP_SSID, AP_PASSWORD))
   {
     Serial.println(".");
     delay(100);
   }
    printWifiHostIP();
  } else if (WiFi.status() == WL_CONNECTED) {
    APMODE = 0;
    CONN_ESTABLISHED = 1;
    printWifiHostIP();
  }
}

void initWiFi() {
  connectionTimeCounter = millis();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
}
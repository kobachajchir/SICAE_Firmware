#pragma once
#include "Globals.h"
#include "LittleFS.h"

void formatLittleFS() {
  Serial.println(F("Formatting LittleFS..."));
  if (LittleFS.format()) {
    Serial.println(F("LittleFS formatted successfully"));
    if (LittleFS.begin()) {
      Serial.println(F("LittleFS mounted successfully after formatting"));
    } else {
      Serial.println(F("Failed to mount LittleFS after formatting"));
    }
  } else {
    Serial.println(F("Failed to format LittleFS"));
  }
}

void initFS() {
  // Inicializar LittleFS
  if (!LittleFS.begin()) {
    Serial.println(F("An error has occurred while mounting LittleFS"));
    formatLittleFS();
  } else {
    Serial.println(F("LittleFS mounted successfully"));
  }
}

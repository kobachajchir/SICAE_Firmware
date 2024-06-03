#pragma once
#include "Globals.h"

void sendJsonDocument(const String& output);

uint8_t cmd_codes[] = {
  160, // 0xA0 - Prueba LCD
  161, // 0xA1 - Prueba Botones
  162, // 0xA2 - Prueba Sensor de Luz
  163, // 0xA3 - Prueba Sensor IR
  164, // 0xA4 - Prueba Transmisión IR
  240, // 0xF0 - Obtener Firmware
  241, // 0xF1 - Obtener Estado
  242, // 0xF2 - Obtener Hora Actual
  243, // 0xF3 - Establecer Hora Actual
  244, // 0xF4 - Establecer Datos
  245, // 0xF5 - Establecer Datos de Conexión
  246, // 0xF6 - Establecer Datos de Autenticación
  247, // 0xF7 - Autenticar
  224, // 0xE0 - Error: General
  225, // 0xE1 - Error: Conexión
  226, // 0xE2 - Error: Autenticación
  227, // 0xE3 - Error: Datos
  208, // 0xD0 - ESP Pide Datos
  209  // 0xD1 - ESP Pide Hora Actual
};
const int num_codes = sizeof(cmd_codes) / sizeof(cmd_codes[0]); // Calculate the number of elements in the array

bool isCommandValid(uint8_t cmd) {
  for (int i = 0; i < num_codes; i++) {
    if (cmd == cmd_codes[i]) {
      return true;
    }
  }
  return false;
}

uint8_t calculateChecksum(JsonObjectConst obj, uint8_t len) {
  uint8_t checksum = 0;
  for (JsonPairConst kv : obj) {
    const char* key = kv.key().c_str();
    if (strcmp(key, "cmd") != 0 && strcmp(key, "cks") != 0 && strcmp(key, "len") != 0) {
      checksum ^= (uint8_t)kv.value().as<int>();
    }else{
      Serial.print("Salteando");
      Serial.println(key);
    }
  }
  return checksum;
}

uint8_t calculateLen(JsonObjectConst obj) {
  uint8_t len = 0;
  for (JsonPairConst kv : obj) {
    const char* key = kv.key().c_str();
    if (strcmp(key, "cmd") != 0 && strcmp(key, "cks") != 0 && strcmp(key, "len") != 0) {
      len++;
    }
  }
  return len;
}

void launchCommand(uint8_t cmd, JsonObjectConst obj) {
    switch (cmd) {
      case 160: // 0xA0 Prueba LCD
          // Código para prueba LCD
          break;
      case 161: // 0xA1 Prueba Botones
          // Código para prueba Botones
          break;
      case 162: // 0xA2 Prueba Sensor de Luz
          // Código para prueba Sensor de Luz
          break;
      case 163: // 0xA3 Prueba Sensor IR
          // Código para prueba Sensor IR
          break;
      case 164: // 0xA4 Prueba Transmisión IR
          // Código para prueba Transmisión IR
          break;
      case 208: // 0xD0 ESP Pide Datos
          // Código para ESP Pide Datos
          break;
      case 209: // 0xD1 ESP Pide Hora Actual
          // Código para ESP Pide Hora Actual
          break;
      case 224: // 0xE0 Error: General
          // Código para Error General
          break;
      case 225: // 0xE1 Error: Conexión
          // Código para Error Conexión
          break;
      case 226: // 0xE2 Error: Autenticación
          // Código para Error Autenticación
          break;
      case 227: // 0xE3 Error: Datos
          // Código para Error Datos
          break;
      case 240: // 0xF0 Obtener Firmware
          // Código para Obtener Firmware
          break;
      case 241: // 0xF1 Obtener Estado
          // Código para Obtener Estado
          break;
      case 242: // 0xF2 Obtener Hora Actual
          // Código para Obtener Hora Actual
          break;
      case 243: // 0xF3 Establecer Hora Actual
          // Código para Establecer Hora Actual
          break;
      case 244: // 0xF4 Establecer Datos
          // Código para Establecer Datos
          break;
      case 245: // 0xF5 Establecer Datos de Conexión
          // Código para Establecer Datos de Conexión
          break;
      case 246: // 0xF6 Establecer Datos de Autenticación
          // Código para Establecer Datos de Autenticación
          break;
      case 247: // 0xF7 Autenticar
          // Código para Autenticar
          break;
    }
}

void processCommand(uint8_t cmd, JsonObjectConst doc) {
  uint8_t len = doc["len"];  // Extract length from JSON
  uint8_t receivedChecksum = doc["cks"];  // Extract checksum from JSON
  uint8_t calculatedChecksum = calculateChecksum(doc, len);  // Calculate checksum based on JSON payload

  if (receivedChecksum == calculatedChecksum) {
    Serial.println("Checksum validation passed");
    launchCommand(cmd, doc);  // Your function to launch the command
  } else {
    Serial.println("Checksum validation failed");
  }
}

/*
void case160Handler() { 
  DynamicJsonDocument doc(128);
  doc["cmd"] = 160;
  String payload;
  serializeJson(doc, payload);
  uint8_t len = calculateLen(doc.as<JsonObjectConst>());
  uint8_t cks = calculateChecksum(doc.as<JsonObjectConst>(), len);
  doc["len"] = len;
  doc["cks"] = cks;
  String output;
  serializeJson(doc, output);
  sendJsonDocument(output);
}
*/
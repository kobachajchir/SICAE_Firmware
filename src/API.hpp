#pragma once
#include "Globals.h"
#include "ArduinoJson.h"
#include "webSockets.hpp" // Incluir webSockets.hpp para tener acceso a ws

// Declaraciones de funciones
void sendJsonDocument(const String& output);
void handleGetSystemInfo(AsyncWebSocketClient *client);
void handleGetConnectionInfo(AsyncWebSocketClient *client);
void handleSetConnectionInfo(AsyncWebSocketClient *client, JsonObjectConst doc);
void handleAuthenticate(AsyncWebSocketClient *client, JsonObjectConst doc);
bool isCommandValid(uint8_t cmd);
void processCommand(uint8_t cmd, JsonObjectConst doc, AsyncWebSocketClient *client);

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

// Función para manejar la solicitud de información del sistema
void handleGetSystemInfo(AsyncWebSocketClient *client) {
  SystemInfo systemInfo = {"1.0.0", "2023-01-01 12:00:00", "ESP32-123456"};
  StaticJsonDocument<200> doc;
  doc["firmware"] = systemInfo.firmware;
  doc["espTime"] = systemInfo.espTime;
  doc["chipId"] = systemInfo.chipId;

  String output;
  serializeJson(doc, output);
  sendJsonDocument(output);
}

// Función para manejar la solicitud de información de conexión
void handleGetConnectionInfo(AsyncWebSocketClient *client) {
  ConnectionInfo connectionInfo = {"Koba", "koba1254", "ESP-AP-123456", "esp1234", false};
  StaticJsonDocument<200> doc;
  doc["wifiSsid"] = connectionInfo.wifiSsid;
  doc["wifiPassword"] = connectionInfo.wifiPassword;
  doc["apSsid"] = connectionInfo.apSsid;
  doc["apPassword"] = connectionInfo.apPassword;
  doc["apMode"] = connectionInfo.apMode;

  String output;
  serializeJson(doc, output);
  sendJsonDocument(output);
}

// Función para manejar la configuración de información de conexión
void handleSetConnectionInfo(AsyncWebSocketClient *client, JsonObjectConst doc) {
  if (!doc.containsKey("wifiSsid") || !doc.containsKey("wifiPassword") ||
      !doc.containsKey("apSsid") || !doc.containsKey("apPassword") ||
      !doc.containsKey("apMode")) {
    Serial.println("Invalid JSON format");
    // Send error response
    StaticJsonDocument<200> errorDoc;
    errorDoc["error"] = "Invalid JSON format";
    errorDoc["code"] = 400;
    String errorOutput;
    serializeJson(errorDoc, errorOutput);
    client->text(errorOutput);
    return;
  }

  ConnectionInfo connectionInfo;
  connectionInfo.wifiSsid = doc["wifiSsid"].as<String>();
  connectionInfo.wifiPassword = doc["wifiPassword"].as<String>();
  connectionInfo.apSsid = doc["apSsid"].as<String>();
  connectionInfo.apPassword = doc["apPassword"].as<String>();
  connectionInfo.apMode = doc["apMode"].as<bool>();

  // Guardar o aplicar la nueva configuración según sea necesario
  Serial.println("Connection info updated");

  // Responder con el estado actualizado
  handleGetConnectionInfo(client);
}


// Función para manejar la autenticación
void handleAuthenticate(AsyncWebSocketClient *client, JsonObjectConst doc) {
  UserInfo userInfo;
  userInfo.userEmail = doc["userEmail"].as<String>();
  userInfo.userPassword = doc["userPassword"].as<String>();
  userInfo.prefersDarkMode = doc["prefersDarkMode"].as<bool>();

  // Realizar la autenticación y responder con el resultado
  bool authenticated = true; // Ejemplo de autenticación exitosa
  StaticJsonDocument<200> responseDoc;
  responseDoc["authenticated"] = authenticated;

  String output;
  serializeJson(responseDoc, output);
  sendJsonDocument(output);
}

// Función para procesar los comandos recibidos
void processCommand(uint8_t cmd, JsonObjectConst doc, AsyncWebSocketClient *client) {
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
    case 240: // 0xF0 Obtener Firmware
      handleGetSystemInfo(client);
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
      handleSetConnectionInfo(client, doc);
      break;
    case 245: // 0xF5 Establecer Datos de Conexión
      handleSetConnectionInfo(client, doc);
      break;
    case 246: // 0xF6 Establecer Datos de Autenticación
      handleAuthenticate(client, doc);
      break;
    case 247: // 0xF7 Autenticar
      handleAuthenticate(client, doc);
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
    case 208: // 0xD0 ESP Pide Datos
      handleGetSystemInfo(client);
      break;
    case 209: // 0xD1 ESP Pide Hora Actual
      // Código para ESP Pide Hora Actual
      break;
    default:
      Serial.println("Unknown command");
  }
}

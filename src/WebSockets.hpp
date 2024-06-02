#pragma once
#include "ArduinoJson.h"
using namespace ArduinoJson;

bool isCommandValid(uint8_t cmd);
void processCommand(uint8_t cmd, JsonObjectConst doc);

AsyncWebSocket ws("/ws");

void ProcessRequest(AsyncWebSocketClient * client, String request) { //then modify this to make the client a already created pointer
  Serial.println(request);
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, request);
  
  if (error) { 
    Serial.println("Deserialization Error");
    return; 
  }
  
  String commandString = doc["cmd"];
  uint8_t command = (uint8_t)commandString.toInt();

  if (isCommandValid(command)) {
    processCommand(command, doc.as<JsonObjectConst>());
  } else {
    Serial.println("Invalid command received");
  }
}

void sendJsonDocument(const String& jsonOutput) {
  Serial.print("sending response");
  Serial.print(jsonOutput);
  ws.textAll(jsonOutput);
}

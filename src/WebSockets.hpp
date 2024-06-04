#pragma once
#include "ArduinoJson.h"
#include <ESPAsyncWebServer.h> // Asegúrate de tener la biblioteca adecuada
using namespace ArduinoJson;

// Declarar ws como extern para que esté disponible en otros archivos
extern AsyncWebSocket ws;

bool isCommandValid(uint8_t cmd);
void processCommand(uint8_t cmd, JsonObjectConst doc, AsyncWebSocketClient *client);

void ProcessRequest(AsyncWebSocketClient * client, String request) {
  Serial.println(request);
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, request);

  if (error) {
    Serial.println("Deserialization Error");
    // Send error response
    StaticJsonDocument<200> errorDoc;
    errorDoc["error"] = "Deserialization Error";
    errorDoc["code"] = 400;
    String errorOutput;
    serializeJson(errorDoc, errorOutput);
    client->text(errorOutput);
    return;
  }

  String commandString = doc["cmd"];
  uint8_t command = (uint8_t)commandString.toInt();

  if (isCommandValid(command)) {
    processCommand(command, doc.as<JsonObjectConst>(), client);
  } else {
    Serial.println("Invalid command received");
    // Send error response
    StaticJsonDocument<200> errorDoc;
    errorDoc["error"] = "Invalid Command";
    errorDoc["code"] = 400;
    String errorOutput;
    serializeJson(errorDoc, errorOutput);
    client->text(errorOutput);
  }
}

void sendJsonDocument(const String& jsonOutput) {
  Serial.print("sending response");
  Serial.print(jsonOutput);
  ws.textAll(jsonOutput);
}

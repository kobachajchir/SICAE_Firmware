#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "LittleFS.h"

// Estructuras para la información del sistema, conexión y usuario
struct SystemInfo {
  String firmware;
  String espTime;
  String chipId;
};

struct ConnectionInfo {
  String wifiSsid;
  String wifiPassword;
  String apSsid;
  String apPassword;
  bool apMode;
};

struct UserInfo {
  String userEmail;
  String userPassword;
  bool prefersDarkMode;
};

// Declaración de variables globales
ConnectionInfo connectionInfo = {"Koba", "koba1254", "ESP-AP-123456", "esp1234", false};
AsyncWebServer server(80);

// Función para manejar rutas no encontradas
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// Función para inicializar el servidor
void InitServer() {
  // Configurar rutas del servidor después de determinar el estado de APMODE

  // Ignorar solicitudes para favicon.ico
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(204);  // 204 No Content
  });

  // Ruta para el archivo JavaScript bundle
  server.on("/bundle.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/bundle.js", "application/javascript");
  });

  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/styles.css", "text/css");
  });


  server.on("/connectionInfo", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<200> doc;
    doc["wifiSsid"] = connectionInfo.wifiSsid;
    doc["wifiPassword"] = connectionInfo.wifiPassword;
    doc["apSsid"] = connectionInfo.apSsid;
    doc["apPassword"] = connectionInfo.apPassword;
    doc["apMode"] = connectionInfo.apMode;
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  // Ruta para actualizar la información de conexión
  server.on("/connectionInfo", HTTP_POST, [](AsyncWebServerRequest *request) {
  // Leer el cuerpo de la solicitud
  AsyncWebServerResponse *response = request->beginResponseStream("application/json");
  DynamicJsonDocument doc(1024);

  if (request->hasParam("plain", true)) {
    String body = request->getParam("plain", true)->value();
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      request->send(400, "application/json", "{\"message\":\"Invalid JSON\"}");
      return;
    }

    if (doc.containsKey("wifiSsid") && doc.containsKey("wifiPassword") &&
        doc.containsKey("apSsid") && doc.containsKey("apPassword") &&
        doc.containsKey("apMode")) {

      connectionInfo.wifiSsid = doc["wifiSsid"].as<String>();
      connectionInfo.wifiPassword = doc["wifiPassword"].as<String>();
      connectionInfo.apSsid = doc["apSsid"].as<String>();
      connectionInfo.apPassword = doc["apPassword"].as<String>();
      connectionInfo.apMode = doc["apMode"].as<bool>();

      FLAG_UPDATE_CONFIG = 1;
      request->send(200, "application/json", "{\"message\":\"Connection info updated\"}");

      Serial.println("Received request:");
      Serial.print("wifiSsid: ");
      Serial.println(connectionInfo.wifiSsid);
      Serial.print("wifiPassword: ");
      Serial.println(connectionInfo.wifiPassword);
      Serial.print("apSsid: ");
      Serial.println(connectionInfo.apSsid);
      Serial.print("apPassword: ");
      Serial.println(connectionInfo.apPassword);
      Serial.print("apMode: ");
      Serial.println(connectionInfo.apMode ? "true" : "false");

    } else {
      request->send(400, "application/json", "{\"message\":\"Missing parameters\"}");
    }
  } else {
    request->send(400, "application/json", "{\"message\":\"Missing body\"}");
  }
});




  server.onNotFound(notFound);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", F("text/html"));
  });

  server.serveStatic("/", LittleFS, "/");

  server.begin();
  Serial.println("Server started");
}

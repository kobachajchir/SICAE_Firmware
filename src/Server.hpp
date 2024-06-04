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
  server.on("/connectionInfo", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("wifiSsid", true) && request->hasParam("wifiPassword", true) &&
        request->hasParam("apSsid", true) && request->hasParam("apPassword", true) &&
        request->hasParam("apMode", true)) {
      connectionInfo.wifiSsid = request->getParam("wifiSsid", true)->value();
      connectionInfo.wifiPassword = request->getParam("wifiPassword", true)->value();
      connectionInfo.apSsid = request->getParam("apSsid", true)->value();
      connectionInfo.apPassword = request->getParam("apPassword", true)->value();
      connectionInfo.apMode = request->getParam("apMode", true)->value().equalsIgnoreCase("true");

      request->send(200, "text/plain", "Connection info updated");
    } else {
      request->send(400, "text/plain", "Bad Request");
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

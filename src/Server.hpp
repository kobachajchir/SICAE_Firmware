AsyncWebServer server(80);

void InitServer()
{
  // Configurar rutas del servidor después de determinar el estado de APMODE
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (APMODE) {
      request->send(LittleFS, "/pageData/indexAP.html", F("text/html"));
    } else {
      request->send(LittleFS, "/pageData/index.html", F("text/html"));
    }
  });

  // Ruta para el archivo JavaScript bundle
  server.on("/bundle.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/pageData/bundle.js", "application/javascript");
  });

  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/pageData/styles.css", "text/css");
  });

  server.serveStatic("/", LittleFS, "/");

  server.begin();
  Serial.print("Server started");
}
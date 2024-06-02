#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "LittleFS.h"

// Proporciona la información del proceso de generación de tokens.
#include "addons/TokenHelper.h"
// Proporciona la información de impresión de la carga útil de RTDB y otras funciones auxiliares.
#include "addons/RTDBHelper.h"

// Inserta tus credenciales de red
#define WIFI_SSID "Koba"
#define WIFI_PASSWORD "koba1254"

// Inserta la clave API del proyecto de Firebase
#define API_KEY "AIzaSyAX0p4VIdtfN7I7dnaOGpBtfGAtlG3IqDY"

// Inserta la URL de la base de datos de Firebase
#define DATABASE_URL "https://sicaewebapp-default-rtdb.firebaseio.com/"

// Inserta el correo electrónico autorizado y la contraseña correspondiente
#define USER_EMAIL "koba@test.com"
#define USER_PASSWORD "koba1254"

// Define el objeto de datos de Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable para guardar el UID del usuario
String uid;
// Variable para guardar el chip ID
String path = "/dispositivos/";
String ssidAP = "ESP-AP-";
String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
String firmware = "0.1";

// Definiciones de tiempo y bandera
#define WIFI_WAIT_TIME 5000
#define FIREBASE_UPDATE_TIME 30000

typedef union {
  struct {
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
    uint8_t bit4 : 1;
    uint8_t bit5 : 1;
    uint8_t bit6 : 1;
    uint8_t bit7 : 1;
  } bits;
  uint8_t byte;
} myByte;

myByte flag;

#define APMODE flag.bits.bit0
#define CONN_ESTABLISHED flag.bits.bit1
#define WRITE_TO_FIREBASE flag.bits.bit2
#define READ_FROM_FIREBASE flag.bits.bit3
#define UPDATED flag.bits.bit4

uint32_t sendDataPrevMillis = 0;
uint32_t connectionTimeCounter = 0;
bool signupOK = false;
FirebaseJson content;
FirebaseJsonData jsonData;

// Crear objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);

// Crear objeto WebSocket
AsyncWebSocket ws("/ws");

// Declaraciones de funciones
void initFS();
void formatLittleFS();
void initWebSocket();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void notifyClients(String message);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void updateFirebaseEntry(FirebaseJson& content);
void initWiFi();
void checkWiFiConnection();

void initFS() {
  // Inicializar LittleFS
  if (!LittleFS.begin()) {
    Serial.println(F("An error has occurred while mounting LittleFS"));
    formatLittleFS();
  } else {
    Serial.println(F("LittleFS mounted successfully"));
  }
}

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

void initWebSocket() {
  // Inicializar WebSocket
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  Serial.println("WebSockets started");
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    // Aquí puedes manejar los mensajes recibidos por WebSocket
  }
}

void notifyClients(String message) {
  ws.textAll(message);
}

void initWiFi() {
  connectionTimeCounter = millis();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED && (millis() - connectionTimeCounter) >= WIFI_WAIT_TIME) {
    APMODE = 1;
    CONN_ESTABLISHED = 1;
    Serial.println(F("\nNo se pudo conectar a Wi-Fi. Iniciando modo AP..."));
    WiFi.softAP(ssidAP.c_str());
    Serial.print(F("AP SSID: "));
    Serial.println(ssidAP);
    Serial.print(F("IP Address: "));
    Serial.println(WiFi.softAPIP());
  } else if (WiFi.status() == WL_CONNECTED) {
    APMODE = 0;
    CONN_ESTABLISHED = 1;
    Serial.println(F("\nConectado a Wi-Fi."));
    Serial.print(F("IP Address: "));
    Serial.println(WiFi.localIP());
  }
}

void updateFirebaseEntry(FirebaseJson& content) {
  Serial.print("Updating node at path: ");
  Serial.println(path);

  if (Firebase.RTDB.updateNode(&fbdo, path.c_str(), &content)) {
    Serial.println("Data updated in Firebase");
  } else {
    Serial.print("Update failed: ");
    Serial.println(fbdo.errorReason());
  }
}

void setup() {
  //Keep these comment and line on top of setup
  ssidAP.concat(chipId);
  path.concat(chipId);
  //Until here on top
  
  Serial.begin(115200);

  // Inicializa LittleFS
  initFS();

  // Inicializa WiFi
  initWiFi();

  Serial.print("Chip ID:");
  Serial.println(chipId);

  // Asigna la clave API y la URL de RTDB
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Asigna las credenciales de inicio de sesión del usuario
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Asigna la función de callback para la tarea de generación de tokens de larga duración
  config.token_status_callback = tokenStatusCallback; // Ver addons/TokenHelper.h

  // Asigna el máximo número de reintentos para la generación de tokens
  config.max_token_generation_retry = 5;

  while (!CONN_ESTABLISHED){
    checkWiFiConnection();
    Serial.print(".");
    delay(100);
  }

  // Inicializa la biblioteca con la autenticación y configuración de Firebase
  Firebase.begin(&config, &auth);
  Serial.print("Firebase started");

  // Verifica si la conexión a Firebase es exitosa
  if (!Firebase.ready()) {
    Serial.println("Failed to connect to Firebase");
    Serial.println(fbdo.errorReason());
    return;
  }
  Serial.print("Firebase ready");


  // Obtener el UID del usuario puede tardar unos segundos
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Imprime el UID del usuario
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Configurar rutas del servidor después de determinar el estado de APMODE
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (APMODE) {
      request->send(LittleFS, "/indexAP.html", F("text/html"));
    } else {
      request->send(LittleFS, "/index.html", F("text/html"));
    }
  });

  // Ruta para el archivo JavaScript bundle
  server.on("/bundle.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/bundle.js", "application/javascript");
  });

  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/styles.css", "text/css");
  });

  server.serveStatic("/", LittleFS, "/");

  server.begin();
  Serial.print("Server started");
  initWebSocket();
  READ_FROM_FIREBASE = 1;
}

void loop() {
  if(Firebase.ready()){
    if((millis() - sendDataPrevMillis)>FIREBASE_UPDATE_TIME) { //Every 30sec TEST ONLY
      WRITE_TO_FIREBASE = 1;
    }
    if(READ_FROM_FIREBASE){
      READ_FROM_FIREBASE = 0;
      if (Firebase.RTDB.getJSON(&fbdo, path.c_str())) {
        if (fbdo.dataType() == "json") {
          FirebaseJson &currentData = fbdo.jsonObject();
          String currentIp, currentFirmware, currentWifiName;
          bool currentOnline;

          if (currentData.get(jsonData, "currentIp") && jsonData.type == "string") {
            currentIp = jsonData.stringValue;
          } else {
            currentIp = "";
          }

          if (currentData.get(jsonData, "firmware") && jsonData.type == "string") {
            currentFirmware = jsonData.stringValue;
          } else {
            currentFirmware = "";
          }

          if (currentData.get(jsonData, "wifiName") && jsonData.type == "string") {
            currentWifiName = jsonData.stringValue;
          } else {
            currentWifiName = "";
          }

          if (currentData.get(jsonData, "online") && jsonData.type == "boolean") {
            currentOnline = jsonData.boolValue;
          } else {
            currentOnline = false;
          }

          // Comparar y actualizar solo si es necesario
          if (currentIp != WiFi.localIP().toString()){
            content.set("currentIp", WiFi.localIP().toString());
            WRITE_TO_FIREBASE = 1;
          }
          if (currentFirmware != firmware){
            content.set("firmware", firmware);
            WRITE_TO_FIREBASE = 1;
          }
          if (currentWifiName != WIFI_SSID){
            content.set("wifiName", WIFI_SSID);
            WRITE_TO_FIREBASE = 1;
          }
          if (currentOnline != true){
            content.set("online", true);
            WRITE_TO_FIREBASE = 1;
          } 
        } else {
          Serial.println("Creating new ESP32 File on DB");
          content.set("currentIp", WiFi.localIP().toString());
          content.set("firmware", firmware);
          content.set("online", true);
          content.set("salaName", "Sala 1");
          content.set("wifiName", WIFI_SSID);
          content.set("apMode", flag.bits.bit0);
          content.set("apSSID", ssidAP);

          FirebaseJson data;
          FirebaseJson irData;
          irData.set("up", "");
          irData.set("down", "");
          irData.set("power", "");

          data.set("deviceType", "");
          data.set("icon", "");
          data.set("irData", irData);
          data.set("model", "");
          data.set("name", "");
          data.set("state", false);

          content.set("data/[0]", data); //Here is not 0 fixed, is the number it corresponds to the amount of data items in the content 
          WRITE_TO_FIREBASE = 1;
        }
      } else {
        Serial.println(fbdo.errorReason());
      }
    }
    if(WRITE_TO_FIREBASE){
      WRITE_TO_FIREBASE = 0;
      sendDataPrevMillis = millis();
      if (Firebase.RTDB.set(&fbdo, path.c_str(), &content)) {
        Serial.println("Initial data sent to Firebase");
        UPDATED = 1;
      } else {
        Serial.print("Set request failed: ");
        Serial.println(fbdo.errorReason());
      }
    }
  }

  if (Firebase.isTokenExpired()) {
    Firebase.refreshToken(&config);
    Serial.println("Refresh token");
  }

  ws.cleanupClients();
}

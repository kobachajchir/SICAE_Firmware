#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include "FS.h"
#include <LittleFS.h>
#include <LCDI2C_Multilingual.h>
#include "Globals.h"
#include "myByte.hpp"
#include "LCDControl.hpp"
#include "WebSockets.hpp"
#include "Server.hpp"
#include "ESP32_Utils.hpp"
#include "ESP32_Utils_AWS.hpp"
#include "Utils_FS.hpp"
#include "API.hpp"
// Proporciona la información del proceso de generación de tokens.
#include "addons/TokenHelper.h"
// Proporciona la información de impresión de la carga útil de RTDB y otras funciones auxiliares.
#include "addons/RTDBHelper.h"

// I2C (LCD Display)
const uint8_t sdaPin = 21;
const uint8_t sclPin = 22;

// Define el objeto de datos de Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

IPAddress ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Variable para guardar el UID del usuario
String uid;
// Variable para guardar el chip ID
String path = "/dispositivos/";
String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
String firmware = "0.1";
String ssidAP;
String apPassword;
String wifiSsid;
String wifiPassword;
String apiKey;
String databaseUrl;
String userEmail;
String userPassword;

myByte flag;

uint32_t lastTime = 0;
uint32_t sendDataPrevMillis = 0;
uint32_t timerLCD = 0;
uint32_t connectionTimeCounter = 0;
uint8_t lcdClearTimer = 0;
bool signupOK = false;
FirebaseJson content;
FirebaseJsonData jsonData;
uint16_t msCounter;
uint16_t msCounterCount;

// Declaraciones de funciones
void updateFirebaseEntry(FirebaseJson& content);
void readConfigFile();

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

void readConfigFile() {
  String fileContent = readFile(LittleFS, "/config.txt");
      Serial.println("File Content:");
    Serial.println(fileContent);
  if (fileContent.length() > 0) {
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, fileContent);
    if (error) {
      Serial.println("Failed to parse config file");
      return;
    }
    // Asignar valores a las variables globales
    wifiSsid = doc["ssid"].as<String>();
    wifiPassword = doc["password"].as<String>();
    ssidAP = doc["apSsid"].as<String>();
    apPassword = doc["apPassword"].as<String>();  // Esto no se usa en el código actual
    userEmail = doc["userEmail"].as<String>();
    userPassword = doc["userPassword"].as<String>();
    databaseUrl = doc["databaseUrl"].as<String>();
    apiKey = doc["appKey"].as<String>();
    Serial.print("WiFi SSID: ");
    Serial.println(wifiSsid);
    Serial.print("WiFi Password: ");
    Serial.println(wifiPassword);
    Serial.print("API Key: ");
    Serial.println(apiKey);
    Serial.print("Database URL: ");
    Serial.println(databaseUrl);
    Serial.print("User Email: ");
    Serial.println(userEmail);
    Serial.print("User Password: ");
    Serial.println(userPassword);
  } else {
    Serial.println("Config file is empty or not found");
  }
}

void setup() {
  //Keep these comment and line on top of setup
  ssidAP.concat(chipId);
  path.concat(chipId);
  //Until here on top
  
  Serial.begin(115200);
    // set up the LCD's number of rows and columns:
      // Inicializa LittleFS
  initFS();
  bool fileexists = LittleFS.exists("/config.txt");
  if (!fileexists) {
    Serial.println("File doesn’t exist");
    Serial.println("Creating file...");

    // Crear objeto JSON
    DynamicJsonDocument doc(2048);  // Aumentar el tamaño del documento JSON

    doc["ssid"] = "Koba";
    doc["password"] = "koba1254";
    doc["apSsid"] = "ESP-AP-9c842178";
    doc["apPassword"] = "esp1234";
    doc["userEmail"] = "koba@test.com";
    doc["userPassword"] = "koba1254";
    doc["databaseUrl"] = "https://sicaewebapp-default-rtdb.firebaseio.com/";
    doc["appKey"] = "AIzaSyAX0p4VIdtfN7I7dnaOGpBtfGAtlG3IqDY";

    // Añadir datos adicionales en la estructura requerida
    JsonObject data0 = doc.createNestedObject("data[0]");
    data0["deviceType"] = "";
    data0["icon"] = "";
    JsonObject irData = data0.createNestedObject("irData");
    irData["down"] = "";
    irData["power"] = "";
    irData["up"] = "";
    data0["model"] = "";
    data0["name"] = "";
    data0["state"] = false;

    // Convertir JSON a cadena
    String jsonString;
    serializeJson(doc, jsonString);
    // Escribir JSON en el archivo
    writeFile(LittleFS, "/config.txt", jsonString.c_str());
  } else {
    Serial.println("File already exists");
    // Leer el contenido del archivo
    readConfigFile();
  }
  while(!Wire.begin(21, 22)){

  }
  LCD_ON = 1;
  timerLCD = 0;
  Serial.print("Iniciando LCD: ");
  lcd.init();
  lcd.backlight();
  lcd.home();
  strcpy(linea, "INICIANDO");
  imprimirLCD(linea, 0);
  initWiFi();
  lastTime = millis();
  msCounter = 0;

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
  InitServer();
	InitWebSockets();
  READ_FROM_FIREBASE = 1;
}

void loop() {
  if((millis()-lastTime) > MINMSTIME){
    lastTime = millis();
    if(LCD_ON){
      if(timerLCD <= LCD_ON_TIME){
        timerLCD++;
      }else{
        timerLCD = LCD_ON_TIME;
        LCD_ON = 0;
        lcd.noBacklight();
      }
    }
    if(LCDWILLCLEAR){
      if(lcdClearTimer >= LCDCLEARTIME){
        lcdClearTimer++;
      }else{
        lcdClearTimer = 0;
        lcd.clear();
        LCDWILLCLEAR = 0;
      }
    }
  }
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
      strcpy(linea, "Subiendo data");
      imprimirLCD(linea, 0);
      strcpy(linea, "");
      imprimirLCD(linea, 1);
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

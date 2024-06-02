#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include "LittleFS.h"
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
String ssidAP = "ESP-AP-";
String chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
String firmware = "0.1";

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
    // set up the LCD's number of rows and columns:
      // Inicializa LittleFS
  initFS();
  while(!Wire.begin(21, 22)){

  }
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000); 
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

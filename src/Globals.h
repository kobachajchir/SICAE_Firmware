#pragma once
#include <WiFi.h>
#include "myByte.hpp"
#include <LiquidCrystal_I2C.h>  

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

// Definiciones de tiempo y bandera
#define MINMSTIME 10
#define WIFI_WAIT_TIME 5000
#define FIREBASE_UPDATE_TIME 30000
#define LCD_ON_TIME 500
#define LCDCLEARTIME 500

#define APMODE flag.bits.bit0
#define CONN_ESTABLISHED flag.bits.bit1
#define WRITE_TO_FIREBASE flag.bits.bit2
#define READ_FROM_FIREBASE flag.bits.bit3
#define UPDATED flag.bits.bit4
#define LCD_ON flag.bits.bit5
#define LCDWILLCLEAR flag.bits.bit6

extern myByte flag;
extern LiquidCrystal_I2C lcd;
extern char linea[16];
extern char carac;
extern uint32_t lastTime;
extern uint32_t timerLCD;
extern uint32_t sendDataPrevMillis;
extern String ssidAP;
extern uint32_t connectionTimeCounter;
extern uint8_t lcdClearTimer;


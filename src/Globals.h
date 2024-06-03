#pragma once
#include <WiFi.h>
#include "myByte.hpp"
#include <LiquidCrystal_I2C.h>

// Definiciones de tiempo y bandera
#define MINMSTIME 10
#define WIFI_WAIT_TIME 5000
#define FIREBASE_UPDATE_TIME 30000
#define LCD_ON_TIME 500
#define LCDCLEARTIME 500

#define PIN_IR_RECEIVER 18
#define PIN_IR 19
#define PIN_BTN_ENTER 39
#define PIN_BTN_UP 34
#define PIN_BTN_DOWN 35

#define APMODE flag.bits.bit0
#define CONN_ESTABLISHED flag.bits.bit1
#define WRITE_TO_FIREBASE flag.bits.bit2
#define READ_FROM_FIREBASE flag.bits.bit3
#define UPDATED flag.bits.bit4
#define LCD_ON flag.bits.bit5
#define LCDWILLCLEAR flag.bits.bit6

#define BTN_UP_PRESSED btnFlag.bits.bit0 //Button Up pressed
#define BTN_UP_RELEASED btnFlag.bits.bit1 //Button Up released
#define BTN_DOWN_PRESSED btnFlag.bits.bit2 //Button Down pressed
#define BTN_DOWN_RELEASED btnFlag.bits.bit3 //Button Down released
#define BTN_ENTER_PRESSED btnFlag.bits.bit4 //Button OK/Back pressed
#define BTN_ENTER_RELEASED btnFlag.bits.bit5 //Button OK/Back released

//MODES DEFINITIONS
#define HOME 0 //Home screen
#define SETTINGS 1 //Change any setting
#define STAGE 2 //Change stage

//MENU ITEMS DEFINITIONS
#define HOMEMENU_MAX 0 //Max number of items in home menu
#define SETTINGSMENU_MAX 6 //Max number of items in settings menu
#define STAGEMENU_MAX 1 //Max number of items in stage menu

//HOME MENU ITEMS DEFINITIONS
#define STANDBY 0 //Standby mode
//SETTINGS MENU ITEMS DEFINITIONS
#define LIGHTS 0 //Change lights
#define WATER 1 //Change water
#define TEMP 2 //Change temperature
#define HUM 3 //Change humidity
#define VENT 4 //Change vent
#define RESETINDOOR 5 //Reset indoor values
//STAGE MENU ITEMS DEFINITIONS
#define CHANGESTAGE 0 //Change to vegetation stage

extern myByte flag;
extern myByte btnFlag;
extern LiquidCrystal_I2C lcd;
extern char linea[16];
extern char carac;
extern uint32_t lastTime;
extern uint32_t timerLCD;
extern uint32_t sendDataPrevMillis;
extern String ssidAP;
extern uint32_t connectionTimeCounter;
extern uint8_t lcdClearTimer;
extern String ssidAP;
extern String apPassword;
extern String wifiSsid;
extern String wifiPassword;
extern String apiKey;
extern String databaseUrl;
extern String userEmail;
extern String userPassword;

// Inserta tus credenciales de red
#define WIFI_SSID wifiSsid
#define WIFI_PASSWORD wifiPassword
#define AP_SSID ssidAP
#define AP_PASSWORD apPassword

// Inserta la clave API del proyecto de Firebase
#define API_KEY apiKey

// Inserta la URL de la base de datos de Firebase
#define DATABASE_URL databaseUrl

// Inserta el correo electrónico autorizado y la contraseña correspondiente
#define USER_EMAIL userEmail
#define USER_PASSWORD userPassword

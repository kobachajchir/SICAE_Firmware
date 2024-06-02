#pragma once
#include "Globals.h"

void sendJsonDocument(const String& output);

uint8_t cmd_codes[] = {160, 161, 177, 193, 194, 195, 196, 197, 209, 210, 211, 212, 225, 241, 242, 243, 244, 245};
const int num_codes = sizeof(cmd_codes) / sizeof(cmd_codes[0]); // Calculate the number of elements in the array

bool isCommandValid(uint8_t cmd) {
  for (int i = 0; i < num_codes; i++) {
    if (cmd == cmd_codes[i]) {
      return true;
    }
  }
  return false;
}

uint8_t calculateChecksum(JsonObjectConst obj, uint8_t len) {
  uint8_t checksum = 0;
  for (JsonPairConst kv : obj) {
    const char* key = kv.key().c_str();
    if (strcmp(key, "cmd") != 0 && strcmp(key, "cks") != 0 && strcmp(key, "len") != 0) {
      checksum ^= (uint8_t)kv.value().as<int>();
    }else{
      Serial.print("Salteando");
      Serial.println(key);
    }
  }
  return checksum;
}

uint8_t calculateLen(JsonObjectConst obj) {
  uint8_t len = 0;
  for (JsonPairConst kv : obj) {
    const char* key = kv.key().c_str();
    if (strcmp(key, "cmd") != 0 && strcmp(key, "cks") != 0 && strcmp(key, "len") != 0) {
      len++;
    }
  }
  return len;
}

/*
void case160Handler() { 
  DynamicJsonDocument doc(128);
  doc["cmd"] = 160;
  String payload;
  serializeJson(doc, payload);
  uint8_t len = calculateLen(doc.as<JsonObjectConst>());
  uint8_t cks = calculateChecksum(doc.as<JsonObjectConst>(), len);
  doc["len"] = len;
  doc["cks"] = cks;
  String output;
  serializeJson(doc, output);
  sendJsonDocument(output);
}
*/

void launchCommand(uint8_t cmd, JsonObjectConst obj) {
    switch (cmd) {
      case 160: // 0xA0 GetAliveStatus
          break;
      case 161: // 0xA1 GetBoardInfo
          break;
      case 177: // 0xB1 MakeZeroWheels
          // Your code here
          break;
      case 193: // 0xC1 GoForward
          break;
      case 194: // 0xC2 GoBackwards
          break;
      case 195: // 0xC3 TurnCW
          // Your code here
          break;
      case 196: // 0xC4 TurnCCW
          // Your code here
          break;
      case 197: // 0xC5 LightsChange
          // Your code here
          break;
      case 209: // 0xD1 OdometryRefreshTimeChange
          // Your code here
          break;
      case 210: // 0xD2 WifiChange
          // Your code here
          break;
      case 211: // 0xD3 MPUReading
          // Your code here
          break;
      case 212: // 0xD4 ConfigData
        break;
      case 225: // 0xE1 ErrorWifiConn
          break;
      case 241: // 0xF1 TestMotores

          break;
      case 242: // 0xF2 TestEncoders

          break;
      case 243: // 0xF3 TestServo
          break;
      case 244: // 0xF4 TestInfrarojos
          
          break;
      case 245: // 0xF5 TestUltrasonido
          break;
      }
}

void processCommand(uint8_t cmd, JsonObjectConst doc) {
  uint8_t len = doc["len"];  // Extract length from JSON
  uint8_t receivedChecksum = doc["cks"];  // Extract checksum from JSON
  uint8_t calculatedChecksum = calculateChecksum(doc, len);  // Calculate checksum based on JSON payload

  if (receivedChecksum == calculatedChecksum) {
    Serial.println("Checksum validation passed");
    launchCommand(cmd, doc);  // Your function to launch the command
  } else {
    Serial.println("Checksum validation failed");
  }
}
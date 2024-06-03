#pragma once
#include "Globals.h"
#include "LittleFS.h"

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void formatLittleFS() {
  Serial.println(F("Formatting LittleFS..."));
  if (LittleFS.format()) {
    Serial.println(F("LittleFS formatted successfully"));
    if (LittleFS.begin()) {
      Serial.println(F("LittleFS mounted successfully after formatting"));
      listDir(LittleFS, "/", 1); // Listar archivos después de formatear y montar LittleFS
    } else {
      Serial.println(F("Failed to mount LittleFS after formatting"));
    }
  } else {
    Serial.println(F("Failed to format LittleFS"));
  }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

// Función para leer un archivo
String readFile(fs::FS &fs, const char * path) {
  File file = fs.open(path);
  if(!file) {
    Serial.println("Failed to open file for reading");
    return String();
  }

  String fileContent;
  while(file.available()) {
    fileContent += String((char)file.read());
  }
  file.close();
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

void initFS() {
  // Inicializar LittleFS
  if (!LittleFS.begin()) {
    Serial.println(F("An error has occurred while mounting LittleFS"));
    formatLittleFS();
  } else {
    Serial.println(F("LittleFS mounted successfully"));
    listDir(LittleFS, "/", 1); // Listar archivos después de montar LittleFS
  }
}
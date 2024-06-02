#include "LCDControl.hpp"
#include "Globals.h"

char linea[16];
char carac;
LiquidCrystal_I2C lcd(0x27, 16, 2); 
IPAddress ipHost;  // Define ipHost here

char my_strlen(const char *p)
{
  const char *start = p;
  while (*p)
    p++;
  return (char)(p - start);
}

void imprimirLCD(const char *aImprimir, char altura)
{
  uint8_t largo = my_strlen(aImprimir);  // Assuming my_strlen() is equivalent to standard strlen()
  uint8_t inicio = 8 - (largo / 2) + 1;
  uint8_t availableSpaces = 15 - inicio; // Total columns (16) - 1 for icon - inicio

  // Set cursor position to 1, as 0 is reserved for icon
  lcd.setCursor(1, altura);

  // Print leading spaces
  for (uint8_t i = 0; i < inicio - 1; ++i)
  {
    lcd.print(' ');
  }

  // Print the actual message
  lcd.print(aImprimir);

  // Print trailing spaces
  for (uint8_t i = 0; i < availableSpaces; ++i)
  {
    lcd.print(' ');
  }
  timerLCD = 0;
  LCD_ON = 1;
  lcd.backlight();
}

void printWifiHostIP(){
  timerLCD = 0;
  LCD_ON = 1;
  if(!APMODE){
    strcpy(linea, "WIFI");
    imprimirLCD(linea, 0);
    ipHost = WiFi.localIP();
  }else{
    strcpy(linea, "AP WIFI");
    imprimirLCD(linea, 0);
    ipHost = WiFi.softAPIP();
  }
  snprintf(linea, sizeof(linea), "%d.%d.%d.%d", ipHost[0], ipHost[1], ipHost[2], ipHost[3]);
  imprimirLCD(linea, 1);
}
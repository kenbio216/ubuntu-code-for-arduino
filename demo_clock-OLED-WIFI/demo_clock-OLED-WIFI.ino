#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include "time.h"

#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height

// Declaration for SSD1306 display connected using software I2C (default case):
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

const char* ssid = "Davublood";
const char* password = "123456xuy";
const char* ntpServer = "time.windows.com";
const long gmtOffset_sec = 8 * 3600;
const int daylightOffset_sec = 0;

char dateBuffer[50] = "";
char timeBuffer[50] = "";

int dateScrollPos = 0;

void updateLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  strftime(dateBuffer, sizeof(dateBuffer), "%A, %B %d %Y", &timeinfo);
  strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeinfo);
  
  Serial.println(dateBuffer);
  Serial.println(timeBuffer);
}

void displayLocalTime()
{
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(-dateScrollPos, (SCREEN_HEIGHT/2)-10);
  display.println(dateBuffer);

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor((SCREEN_WIDTH - strlen(timeBuffer)*12)/2, SCREEN_HEIGHT/2);
  display.println(timeBuffer);

  display.display();

  dateScrollPos++;
  if(dateScrollPos > strlen(dateBuffer)*6){
    dateScrollPos = 0;
  }
}


void setup()
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize a NTP client to get time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  updateLocalTime();
}

void loop()
{
  static unsigned long lastUpdateTime = 0;
  if(millis() - lastUpdateTime > 1000){
    lastUpdateTime = millis();
    updateLocalTime();
  }
  
  displayLocalTime();
  delay(150);
}

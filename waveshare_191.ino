#include "rm67162.h"
#include <TFT_eSPI.h>
#include <Wire.h>
#include "SD_MMC.h"
#include "SensorQMI8658.hpp"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

int DISPLAY_REFRESH_INTERVAL = 10;
unsigned long last_display_refresh_ms = 0;


SensorQMI8658 qmi;

IMUdata acc;
IMUdata gyr;


void setupSD() {
  if (!SD_MMC.begin("/sdcard", true)) {  // true = 1-bit mode
    Serial.println("SD_MMC initialization failed!");
    return;
  }
}


void setup() {
  Serial.begin(115200);

  rm67162_init();
  lcd_setRotation(1);
  sprite.createSprite(536, 241);
  drawSprite("GPMech");
  setupIMU();
  setupSD();
}

void loop() {
  unsigned long time_ms = millis(); // or any time source

  if (time_ms >= last_display_refresh_ms + DISPLAY_REFRESH_INTERVAL) {

    if (qmi.getDataReady()) {
      writeImuToScreen();
    }
    last_display_refresh_ms = time_ms;
    // writeTimeToScreen(time_ms);
  }
}

void writeImuToScreen() {
  qmi.getAccelerometer(acc.x, acc.y, acc.z);
  qmi.getGyroscope(gyr.x, gyr.y, gyr.z);

  char buf1[32], buf2[32], buf3[32];
  unsigned int time_ms = millis();
  unsigned int hours     = time_ms / 3600000;
  unsigned int minutes   = (time_ms % 3600000) / 60000;
  unsigned int seconds   = (time_ms % 60000) / 1000;
  unsigned int millisecs = time_ms % 1000;

  sprintf(buf1, "%02u:%02u:%02u.%03u", hours, minutes, seconds, millisecs);
  // snprintf(buf1, sizeof(buf1), "x,y,z: %d", millis());
  snprintf(buf2, sizeof(buf2), "%+4.2f,%+4.2f,%+4.2f", acc.x, acc.y, acc.z);
  snprintf(buf3, sizeof(buf3), "%+04d,%+04d,%+04d", (int)gyr.x, (int)gyr.y, (int)gyr.z);

  sprite.fillSprite(TFT_BLACK);
  
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.setTextSize(5);	
  sprite.drawString(buf1, 10, 50);
  sprite.drawString(buf2, 10, 100);
  sprite.setTextSize(3);	
  sprite.drawString(buf3, 10, 150);

  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());

  // Serial.printf("\t\t\t\t > %lu  %.2f *C\n", qmi.getTimestamp(), qmi.getTemperature_C());
}

void writeTimeToScreen(unsigned int time_ms) {
  char buf[32];
  unsigned int minutes = time_ms / 60000;
  unsigned int seconds = (time_ms % 60000) / 1000;
  unsigned int millisecs = time_ms % 1000;

  char timeStr[12]; // "mm:ss.sss" + null terminator
  sprintf(timeStr, "%02u:%02u.%03u", minutes, seconds, millisecs);

  drawSprite(timeStr);  // Pass it to your sprite renderer
}

void drawSprite(const char* text) {
  sprite.fillSprite(TFT_BLACK);
  
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.setTextSize(5);	
  sprite.drawString(text, 50, 90);
 
  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
}

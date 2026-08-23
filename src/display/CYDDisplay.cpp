#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"
#include "types.h"
static TFT_eSPI tft=TFT_eSPI();
extern StatisticsData getStatistics();
void displayBegin(){tft.init();tft.setRotation(TFT_ROTATION);tft.fillScreen(TFT_BLACK);tft.setTextColor(TFT_WHITE,TFT_BLACK);tft.setTextSize(2);}
void displayShow(const StatisticsData&s){
  tft.fillScreen(TFT_BLACK);tft.setCursor(8,8);tft.println("WiFiCore");
  tft.setCursor(8,40);tft.printf("APs: %lu",s.aps);
  tft.setCursor(8,65);tft.printf("Clients: %lu",s.clients);
  tft.setCursor(8,90);tft.printf("Frames: %lu",s.frames);
  tft.setCursor(8,115);tft.printf("EAPOL: %lu",s.eapol);
  tft.setCursor(8,140);tft.printf("CH: %u",s.channel);
  tft.setCursor(8,165);tft.printf("RSSI: %d",s.rssi);
  tft.setCursor(8,190);tft.printf("UP: %lus",s.uptime);
}
void displayTask(void*){uint32_t last=0;for(;;){if(millis()-last>=1000){last=millis();displayShow(getStatistics());}vTaskDelay(pdMS_TO_TICKS(50));}}

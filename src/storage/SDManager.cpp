#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "config.h"

static bool sdReady = false;
bool sdManagerBegin() {
  sdReady = SD.begin(SD_CS_PIN);
  if (!sdReady) return false;
  const char* dirs[] = {WIFICORE_DIR,CAPTURE_DIR,SCAN_DIR,STATS_DIR,LOG_DIR,CONFIG_DIR};
  for (auto d : dirs) if (!SD.exists(d)) SD.mkdir(d);
  return true;
}
bool sdIsReady() { return sdReady; }
String sdListJson(const char* path) {
  String out="[";
  File dir=SD.open(path);
  if (!dir || !dir.isDirectory()) return "[]";
  bool first=true;
  File f;
  while ((f=dir.openNextFile())) {
    if (!first) out += ",";
    first=false;
    out += "{\"name\":\""; out += f.name(); out += "\",\"size\":";
    out += String((uint32_t)f.size()); out += ",\"dir\":";
    out += f.isDirectory() ? "true}" : "false}";
    f.close();
  }
  dir.close(); out += "]";
  return out;
}

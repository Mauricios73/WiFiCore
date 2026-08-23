#pragma once

#include <Arduino.h>

// ============================================================
// WiFiCore - Configuration
// ESP32 DevKit / ESP32-2432S028 CYD
// ============================================================

// -------------------- TFT --------------------

#define TFT_SCK       18
#define TFT_MISO      19
#define TFT_MOSI      23
#define TFT_CS        15
#define TFT_DC        2
#define TFT_RST       -1
#define TFT_ROTATION  1

// -------------------- SD ----------------------

#define SD_CS_PIN     5

// -------------------- Audio -------------------

#define BUZZER_PIN    4

// -------------------- WiFi AP -----------------

#define WIFI_AP_SSID      "WiFiCore"
#define WIFI_AP_PASSWORD  "wificore123"
#define WEB_PORT          80

// -------------------- Capture -----------------

#define FRAME_MAX_LEN     512
#define FRAME_QUEUE_LEN   32

// -------------------- Event Bus ---------------

#define EVENT_QUEUE_LEN       32
#define MAX_EVENT_SUBSCRIBERS 8

// -------------------- Channel -----------------

#define CHANNEL_HOP_INTERVAL_MS 500

// -------------------- Statistics -------------

#define STATS_INTERVAL_MS 1000
#define WEB_UPDATE_MS     1000

// -------------------- SD paths ----------------

#define WIFICORE_DIR "/WIFICORE"

#define CAPTURE_DIR "/WIFICORE/CAPTURES"
#define SCAN_DIR    "/WIFICORE/SCANS"
#define STATS_DIR   "/WIFICORE/STATS"
#define LOG_DIR     "/WIFICORE/LOGS"
#define CONFIG_DIR  "/WIFICORE/CONFIG"

#define PCAP_FILE   "/WIFICORE/CAPTURES/current.pcap"
#define SCAN_FILE   "/WIFICORE/SCANS/latest.csv"
#define STATS_FILE  "/WIFICORE/STATS/statistics.csv"
#define LOG_FILE    "/WIFICORE/LOGS/system.log"
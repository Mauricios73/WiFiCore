#pragma once

#include <Arduino.h>
#include "config.h"

// ============================================================
// Frame classification
// ============================================================

enum class FrameKind : uint8_t {
    Unknown = 0,
    Management,
    Control,
    Data,
    EAPOL
};

// ============================================================
// Raw WiFi frame
// ============================================================

struct RawFrame {
    uint16_t len;
    int8_t rssi;
    uint8_t channel;
    uint32_t timestamp;

    uint8_t data[FRAME_MAX_LEN];
};

// ============================================================
// Parsed WiFi event
// ============================================================

struct WiFiEvent {
    uint32_t timestamp;

    uint16_t len;
    int8_t rssi;
    uint8_t channel;

    FrameKind kind;

    uint8_t src[6];
    uint8_t dst[6];
    uint8_t bssid[6];
};

// ============================================================
// Statistics
// ============================================================

struct StatisticsData {
    uint32_t frames;
    uint32_t management;
    uint32_t control;
    uint32_t data;
    uint32_t eapol;

    uint32_t aps;
    uint32_t clients;

    uint8_t channel;
    int8_t rssi;

    uint32_t uptime;
};
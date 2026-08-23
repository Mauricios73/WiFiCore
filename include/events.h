#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "types.h"

// ============================================================
// Event types
// ============================================================

enum class EventType : uint8_t {
    None = 0,
    Frame,
    EAPOL,
    ScanComplete,
    Statistics,
    ChannelChanged,
    System
};

// ============================================================
// Event
// ============================================================

struct Event {
    EventType type;
    uint32_t timestamp;

    union {
        WiFiEvent wifi;
        StatisticsData stats;
    };
};

// ============================================================
// Global queues
// ============================================================

extern QueueHandle_t frameQueue;
extern QueueHandle_t pcapQueue;

// ============================================================
// Event Bus
// ============================================================

bool eventBusBegin();

QueueHandle_t eventSubscribe();

bool eventPublish(
    const Event& event,
    TickType_t wait = 0
);

bool eventReceive(
    QueueHandle_t queue,
    Event& event,
    TickType_t wait = 0
);
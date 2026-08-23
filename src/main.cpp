#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "events.h"
#include "types.h"

// ============================================================
// Core
// ============================================================

bool wifiCaptureBegin();

void frameParserTask(void*);
void channelManagerTask(void*);
void statisticsTask(void*);

// ============================================================
// Storage
// ============================================================

bool sdManagerBegin();
bool sdIsReady();

void pcapWriteHeader();
void pcapTask(void*);

void csvWriteStatistics(
    const StatisticsData&
);

// ============================================================
// Display
// ============================================================

void displayBegin();
void displayTask(void*);

void mascotBegin();

// ============================================================
// Audio
// ============================================================

void audioBegin();

// ============================================================
// Web
// ============================================================

void webPortalBegin();

// ============================================================
// Storage task
// ============================================================

static void storageTask(void*) {

    QueueHandle_t queue =
        eventSubscribe();

    if (!queue) {
        vTaskDelete(nullptr);
        return;
    }

    Event event;

    for (;;) {

        while (
            eventReceive(
                queue,
                event,
                0
            )
        ) {

            if (
                event.type ==
                EventType::Statistics
            ) {

                if (sdIsReady()) {

                    csvWriteStatistics(
                        event.stats
                    );
                }
            }
        }

        vTaskDelay(
            pdMS_TO_TICKS(20)
        );
    }
}

// ============================================================
// Setup
// ============================================================

void setup() {

    Serial.begin(115200);

    delay(500);

    Serial.println();
    Serial.println(
        "================================"
    );
    Serial.println(
        "          WiFiCore"
    );
    Serial.println(
        "================================"
    );

    // Event Bus

    Serial.print(
        "[CORE] EventBus: "
    );

    if (!eventBusBegin()) {

        Serial.println("FAIL");

        while (true) {
            delay(1000);
        }
    }

    Serial.println("OK");

    // Display

    displayBegin();

    // Mascot

    mascotBegin();

    // Audio

    audioBegin();

    // SD

    Serial.print(
        "[SD]   "
    );

    if (sdManagerBegin()) {
        Serial.println("OK");
    } else {
        Serial.println("FAIL");
    }

    // PCAP

    if (sdIsReady()) {
        pcapWriteHeader();
    }

    // WiFi

    Serial.print(
        "[WIFI] "
    );

    if (wifiCaptureBegin()) {
        Serial.println("OK");
    } else {
        Serial.println("FAIL");
    }

    // Web Portal

    webPortalBegin();

    // ========================================================
    // FreeRTOS tasks
    // ========================================================

    xTaskCreatePinnedToCore(
        frameParserTask,
        "FrameParser",
        8192,
        nullptr,
        4,
        nullptr,
        0
    );

    xTaskCreatePinnedToCore(
        channelManagerTask,
        "ChannelManager",
        4096,
        nullptr,
        2,
        nullptr,
        0
    );

    xTaskCreatePinnedToCore(
        statisticsTask,
        "Statistics",
        6144,
        nullptr,
        3,
        nullptr,
        0
    );

    xTaskCreatePinnedToCore(
        storageTask,
        "Storage",
        6144,
        nullptr,
        2,
        nullptr,
        1
    );

    xTaskCreatePinnedToCore(
        pcapTask,
        "PCAPWriter",
        6144,
        nullptr,
        2,
        nullptr,
        1
    );

    xTaskCreatePinnedToCore(
        displayTask,
        "Display",
        6144,
        nullptr,
        1,
        nullptr,
        1
    );

    Serial.println();
    Serial.println(
        "[SYSTEM] WiFiCore started"
    );

    Serial.print(
        "[WEB]    http://"
    );

    // Serial.println(
    //     WiFi.softAPIP()
    // );
}

// ============================================================
// Loop
// ============================================================

void loop() {

    vTaskDelay(
        portMAX_DELAY
    );
}
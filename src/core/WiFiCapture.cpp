#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include "config.h"
#include "events.h"

static volatile uint32_t droppedFrames = 0;

static void IRAM_ATTR promiscuousCallback(
    void* buffer,
    wifi_promiscuous_pkt_type_t type
) {
    if (!buffer || !frameQueue) {
        return;
    }

    const wifi_promiscuous_pkt_t* packet =
        static_cast<wifi_promiscuous_pkt_t*>(buffer);

    uint16_t length =
        packet->rx_ctrl.sig_len;

    if (length > FRAME_MAX_LEN) {
        length = FRAME_MAX_LEN;
    }

    RawFrame frame{};

    frame.len = length;
    frame.rssi = packet->rx_ctrl.rssi;
    frame.channel = packet->rx_ctrl.channel;
    frame.timestamp = micros();

    memcpy(
        frame.data,
        packet->payload,
        length
    );

    BaseType_t higherPriorityTaskWoken =
        pdFALSE;

    if (
        xQueueSendFromISR(
            frameQueue,
            &frame,
            &higherPriorityTaskWoken
        ) != pdTRUE
    ) {
        droppedFrames++;
    }

    if (higherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

bool wifiCaptureBegin() {

    WiFi.mode(WIFI_AP);

    bool apStarted =
        WiFi.softAP(
            WIFI_AP_SSID,
            WIFI_AP_PASSWORD,
            6,
            false,
            4
        );

    if (!apStarted) {
        return false;
    }

    delay(100);

    esp_wifi_set_promiscuous(false);

    esp_wifi_set_promiscuous_rx_cb(
        promiscuousCallback
    );

    esp_wifi_set_channel(
        6,
        WIFI_SECOND_CHAN_NONE
    );

    esp_wifi_set_promiscuous(true);

    return true;
}

uint32_t wifiCaptureDroppedFrames() {
    return droppedFrames;
}
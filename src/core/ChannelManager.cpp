#include <Arduino.h>
#include <esp_wifi.h>

#include "config.h"

static uint8_t currentChannelValue = 6;

void channelManagerTask(void*) {
    // O WiFiCore Web AP utiliza o mesmo rádio do ESP32.
    // Portanto, enquanto o WebPortal estiver ativo,
    // o canal precisa permanecer fixo.

    esp_wifi_set_channel(
        currentChannelValue,
        WIFI_SECOND_CHAN_NONE
    );

    for (;;) {
        vTaskDelay(
            pdMS_TO_TICKS(1000)
        );
    }
}

uint8_t currentChannel() {
    return currentChannelValue;
}
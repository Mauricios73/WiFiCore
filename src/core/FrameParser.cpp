#include <Arduino.h>
#include <string.h>

#include "events.h"
#include "types.h"

// ============================================================
// Frame classification
// ============================================================

static FrameKind classifyFrame(
    const RawFrame& frame
) {
    if (frame.len < 2) {
        return FrameKind::Unknown;
    }

    const uint8_t frameControl = frame.data[0];

    const uint8_t type =
        (frameControl >> 2) & 0x03;

    switch (type) {

        case 0:
            return FrameKind::Management;

        case 1:
            return FrameKind::Control;

        case 2:
            return FrameKind::Data;

        default:
            return FrameKind::Unknown;
    }
}

// ============================================================
// EAPOL detection
// ============================================================

static bool detectEAPOL(
    const RawFrame& frame
) {
    if (frame.len < 24) {
        return false;
    }

    const uint16_t maxSearch =
        frame.len < 64 ? frame.len : 64;

    for (
        uint16_t i = 24;
        i + 7 < maxSearch;
        i++
    ) {
        if (
            frame.data[i]     == 0xAA &&
            frame.data[i + 1] == 0xAA &&
            frame.data[i + 2] == 0x03 &&
            frame.data[i + 3] == 0x00 &&
            frame.data[i + 4] == 0x00 &&
            frame.data[i + 5] == 0x00 &&
            frame.data[i + 6] == 0x88 &&
            frame.data[i + 7] == 0x8E
        ) {
            return true;
        }
    }

    return false;
}

// ============================================================
// MAC extraction
// ============================================================

static void clearMac(
    uint8_t mac[6]
) {
    memset(mac, 0, 6);
}

static void extractAddresses(
    const RawFrame& frame,
    WiFiEvent& event
) {
    clearMac(event.src);
    clearMac(event.dst);
    clearMac(event.bssid);

    if (frame.len < 24) {
        return;
    }

    // 802.11:
    // Address 1 = offset 4
    // Address 2 = offset 10
    // Address 3 = offset 16

    memcpy(
        event.dst,
        frame.data + 4,
        6
    );

    memcpy(
        event.src,
        frame.data + 10,
        6
    );

    memcpy(
        event.bssid,
        frame.data + 16,
        6
    );
}

// ============================================================
// Parser task
// ============================================================

void frameParserTask(void*) {

    RawFrame raw;

    for (;;) {

        if (
            xQueueReceive(
                frameQueue,
                &raw,
                portMAX_DELAY
            ) != pdTRUE
        ) {
            continue;
        }

        // Send a copy to PCAP storage.
        if (pcapQueue) {
            xQueueSend(
                pcapQueue,
                &raw,
                0
            );
        }

        Event event{};

        event.timestamp = millis();

        event.type = EventType::Frame;

        event.wifi.timestamp = raw.timestamp;
        event.wifi.len = raw.len;
        event.wifi.rssi = raw.rssi;
        event.wifi.channel = raw.channel;

        event.wifi.kind =
            classifyFrame(raw);

        extractAddresses(
            raw,
            event.wifi
        );

        // Detect EAPOL.

        if (detectEAPOL(raw)) {

            event.wifi.kind =
                FrameKind::EAPOL;

            event.type =
                EventType::EAPOL;
        }

        eventPublish(event, 0);
    }
}
#include <Arduino.h>
#include <string.h>

#include "events.h"
#include "config.h"

static StatisticsData stats{};

static portMUX_TYPE statisticsMux =
    portMUX_INITIALIZER_UNLOCKED;

// ============================================================
// Known APs / Clients
// ============================================================

static uint8_t apList[64][6];
static uint8_t clientList[128][6];

static uint8_t apCount = 0;
static uint8_t clientCount = 0;

// ============================================================
// MAC utilities
// ============================================================

static bool macExists(
    uint8_t list[][6],
    uint8_t count,
    const uint8_t mac[6]
) {
    for (uint8_t i = 0; i < count; i++) {

        if (
            memcmp(
                list[i],
                mac,
                6
            ) == 0
        ) {
            return true;
        }
    }

    return false;
}

static void addMAC(
    uint8_t list[][6],
    uint8_t& count,
    uint8_t maximum,
    const uint8_t mac[6]
) {
    if (count >= maximum) {
        return;
    }

    if (macExists(list, count, mac)) {
        return;
    }

    memcpy(
        list[count],
        mac,
        6
    );

    count++;
}

// ============================================================
// Public statistics
// ============================================================

StatisticsData getStatistics() {

    StatisticsData result;

    portENTER_CRITICAL(&statisticsMux);

    result = stats;

    result.aps = apCount;
    result.clients = clientCount;

    portEXIT_CRITICAL(&statisticsMux);

    result.uptime =
        millis() / 1000;

    return result;
}

// ============================================================
// Statistics task
// ============================================================

void statisticsTask(void*) {

    QueueHandle_t queue =
        eventSubscribe();

    if (!queue) {
        vTaskDelete(nullptr);
        return;
    }

    Event event;

    uint32_t lastStatistics =
        millis();

    for (;;) {

        while (
            eventReceive(
                queue,
                event,
                0
            )
        ) {

            if (
                event.type != EventType::Frame &&
                event.type != EventType::EAPOL
            ) {
                continue;
            }

            portENTER_CRITICAL(
                &statisticsMux
            );

            stats.frames++;

            stats.rssi =
                event.wifi.rssi;

            stats.channel =
                event.wifi.channel;

            switch (
                event.wifi.kind
            ) {

                case FrameKind::Management:
                    stats.management++;

                    addMAC(
                        apList,
                        apCount,
                        64,
                        event.wifi.src
                    );
                    break;

                case FrameKind::Control:
                    stats.control++;
                    break;

                case FrameKind::Data:

                    stats.data++;

                    addMAC(
                        clientList,
                        clientCount,
                        128,
                        event.wifi.src
                    );

                    addMAC(
                        clientList,
                        clientCount,
                        128,
                        event.wifi.dst
                    );

                    break;

                case FrameKind::EAPOL:

                    stats.eapol++;

                    break;

                default:
                    break;
            }

            portEXIT_CRITICAL(
                &statisticsMux
            );
        }

        // Periodic statistics event.

        uint32_t now = millis();

        if (
            now - lastStatistics >=
            STATS_INTERVAL_MS
        ) {

            lastStatistics = now;

            Event statisticsEvent{};

            statisticsEvent.type =
                EventType::Statistics;

            statisticsEvent.timestamp =
                now;

            statisticsEvent.stats =
                getStatistics();

            eventPublish(
                statisticsEvent,
                0
            );
        }

        vTaskDelay(
            pdMS_TO_TICKS(10)
        );
    }
}
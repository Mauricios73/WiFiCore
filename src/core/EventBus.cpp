#include "events.h"
#include "config.h"

QueueHandle_t frameQueue = nullptr;
QueueHandle_t pcapQueue = nullptr;

static QueueHandle_t subscribers[MAX_EVENT_SUBSCRIBERS];

static uint8_t subscriberCount = 0;

bool eventBusBegin() {
    frameQueue = xQueueCreate(
        FRAME_QUEUE_LEN,
        sizeof(RawFrame)
    );

    pcapQueue = xQueueCreate(
        FRAME_QUEUE_LEN,
        sizeof(RawFrame)
    );

    subscriberCount = 0;

    if (!frameQueue) {
        return false;
    }

    if (!pcapQueue) {
        return false;
    }

    for (uint8_t i = 0; i < MAX_EVENT_SUBSCRIBERS; i++) {
        subscribers[i] = nullptr;
    }

    return true;
}

QueueHandle_t eventSubscribe() {
    if (subscriberCount >= MAX_EVENT_SUBSCRIBERS) {
        return nullptr;
    }

    QueueHandle_t queue = xQueueCreate(
        EVENT_QUEUE_LEN,
        sizeof(Event)
    );

    if (!queue) {
        return nullptr;
    }

    subscribers[subscriberCount] = queue;

    subscriberCount++;

    return queue;
}

bool eventPublish(
    const Event& event,
    TickType_t wait
) {
    bool success = true;

    for (uint8_t i = 0; i < subscriberCount; i++) {
        if (!subscribers[i]) {
            continue;
        }

        if (xQueueSend(
                subscribers[i],
                &event,
                wait
            ) != pdTRUE) {

            success = false;
        }
    }

    return success;
}

bool eventReceive(
    QueueHandle_t queue,
    Event& event,
    TickType_t wait
) {
    if (!queue) {
        return false;
    }

    return xQueueReceive(
        queue,
        &event,
        wait
    ) == pdTRUE;
}
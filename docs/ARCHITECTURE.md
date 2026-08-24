# WiFiCore Architecture

## Overview

WiFiCore is the embedded core of ESPGotchi. It is responsible for collecting wireless telemetry, processing 802.11 data, maintaining device state and publishing information to consumers.

The architecture separates:

- radio/capture
- parsing
- device state
- event transport
- statistics
- persistence
- interfaces
- autonomous behavior

## Data Flow

```text
Wi-Fi Radio
    │
    ▼
Capture Callback
    │
    ▼
Capture Queue
    │
    ▼
Frame Parser
    │
    ├──► AP Manager
    │
    ├──► Client Manager
    │
    └──► EAPOL / protocol analyzers
             │
             ▼
          EventBus
             │
      ┌──────┼────────┐
      ▼      ▼        ▼
 Statistics Storage  AI
      │      │        │
      └──────┼────────┘
             ▼
        TFT / Web
```

## Core Rules

1. The capture callback must remain minimal.
2. Raw driver buffers must not be passed to UI modules.
3. SD writes must be serialized by the storage subsystem.
4. Events should contain stable, owned data rather than pointers to transient radio buffers.
5. Statistics must be derived from Core events.
6. TFT and Web are consumers, not owners of the wireless subsystem.
7. AI receives observations and emits high-level decisions.

## FreeRTOS

The system is expected to use dedicated tasks for work that can block or consume significant CPU time:

- frame parsing
- channel management
- statistics
- storage
- web service
- display refresh
- AI/inference

Task priorities and stack sizes should be tuned only after measuring heap, stack high-water marks and queue pressure on the ESP32-S3.

## EventBus

The EventBus is the communication boundary between producers and consumers.

Typical events include:

```text
FrameCaptured
FrameParsed
AccessPointDetected
AccessPointUpdated
ClientDetected
ClientUpdated
ChannelChanged
HandshakeEvent
StatisticsUpdated
StorageError
SystemStateChanged
```

The final event schema will be defined after the infrastructure phase is stabilized.

## Storage

Persistent storage is intentionally separated from producers. Producers publish records/events; the storage task batches and writes them to SD.

Planned formats:

- PCAP — raw/processed wireless captures
- CSV — tabular network/device/statistics data
- JSON — structured state/configuration
- LOG — diagnostic information

## Interfaces

### TFT

The TFT provides the local embedded interface and mascot presentation.

### Web

The Web interface provides remote access to the same Core state and persistent files without duplicating the wireless logic.

## AI Boundary

The AI layer should consume a compact observation vector derived from stable system state.

```text
Core State
   ↓
Observation / Featurizer
   ↓
AI Policy
   ↓
High-level action
   ↓
Core Controller
```

The AI must not directly manipulate hardware drivers.

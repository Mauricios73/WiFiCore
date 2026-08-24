# ESPGotchi / WiFiCore

<p align="center">
  <strong>Embedded Wi-Fi Security & Wireless Monitoring Platform</strong><br>
  ESP32-based experimentation with 802.11 analysis, autonomous behavior and embedded interfaces.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--S3-E7352C?style=flat-square&logo=espressif&logoColor=white">
  <img src="https://img.shields.io/badge/Framework-Arduino-00979D?style=flat-square&logo=arduino&logoColor=white">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white">
  <img src="https://img.shields.io/badge/Wi--Fi-802.11-1679A7?style=flat-square&logo=wifi&logoColor=white">
  <img src="https://img.shields.io/badge/Status-Development-orange?style=flat-square">
</p>

> **Note:** `WiFiCore` is the repository name. **ESPGotchi** is the project/product name built on top of the WiFiCore architecture.

---

## About

**ESPGotchi / WiFiCore** is a modular embedded platform for Wi-Fi monitoring, wireless security research and experimentation with autonomous behavior on ESP32 hardware.

The project is being redesigned around a strict separation between the **wireless core**, **data processing**, **storage** and **user interfaces**. The long-term goal is to make the device capable of collecting wireless telemetry, parsing 802.11 frames, maintaining AP/client state, generating statistics and exposing the same core through a TFT display and a Web interface.

The project is inspired by the ideas behind Pwnagotchi, but the implementation is being developed specifically for ESP32 hardware and its resource constraints.

---

## Current Hardware Target

The current development target is:

- **ESP32-S3 N16R8**
- **2.8" ILI9341 TFT**
- **Integrated SD-card interface**
- Wi-Fi built into the ESP32-S3
- No audio subsystem in the current architecture

Older code in the repository may still contain references to the previous ESP32/CYD hardware. Hardware migration is intentionally being handled separately from the current documentation cleanup.

---

## Architecture

The central design principle is:

```text
                    ┌──────────────────────┐
                    │      WiFi Radio      │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │   Wi-Fi Capture      │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │   802.11 Parser      │
                    └──────────┬───────────┘
                               │
                    ┌──────────┴───────────┐
                    ▼                      ▼
              ┌───────────┐          ┌───────────┐
              │ AP Manager│          │Client Mgr │
              └─────┬─────┘          └─────┬─────┘
                    │                      │
                    └──────────┬───────────┘
                               ▼
                    ┌──────────────────────┐
                    │       EventBus       │
                    └──────────┬───────────┘
                               │
              ┌────────────────┼────────────────┐
              ▼                ▼                ▼
       ┌─────────────┐  ┌─────────────┐  ┌─────────────┐
       │ Statistics  │  │ SD Storage  │  │ AI / State  │
       └─────────────┘  └─────────────┘  └─────────────┘
              │                │                │
              └────────────────┼────────────────┘
                               ▼
                    ┌──────────────────────┐
                    │      Interfaces      │
                    │     TFT + Web UI     │
                    └──────────────────────┘
```

The capture path should remain lightweight. Expensive processing, storage and presentation must happen outside the radio callback.

---

## Planned Modules

| Module | Responsibility | Status |
|---|---|---|
| WiFi / Radio | Configure and operate the ESP32 Wi-Fi radio | 🚧 |
| Capture | Receive raw 802.11 frames | 🚧 |
| Channel Manager | Channel selection and hopping | 🚧 |
| 802.11 Parser | Parse management/control/data frames | 🚧 |
| AP Manager | Track access points | 🚧 |
| Client Manager | Track stations/clients | 🚧 |
| EventBus | Decouple producers and consumers | 🚧 |
| Statistics | Counters, rates and system telemetry | 🚧 |
| EAPOL | Authorized handshake/frame analysis | 📋 |
| SD Storage | Persistent CSV/PCAP/JSON/log data | 🚧 |
| TFT | Embedded dashboard and mascot | 🚧 |
| Web | Browser dashboard and file management | 🚧 |
| AI | Autonomous state/action experimentation | 📋 |
| Audio | Removed from current target architecture | ❌ |

---

## Development Roadmap

### Phase 1 — Infrastructure

- [ ] Robust EventBus
- [ ] Single StorageTask
- [ ] SD access synchronization
- [ ] Capture queue design
- [ ] Queue overflow handling
- [ ] Dropped-frame accounting
- [ ] Heap monitoring
- [ ] Watchdog integration
- [ ] Global system state

### Phase 2 — Wi-Fi Core

- [ ] RadioManager
- [ ] Real ChannelManager
- [ ] 802.11 parser
- [ ] Frame subtype handling
- [ ] ToDS / FromDS handling
- [ ] QoS handling
- [ ] DeviceManager
- [ ] AP detection and tracking
- [ ] Client detection and tracking

### Phase 3 — EAPOL Analysis

- [ ] LLC/SNAP parser
- [ ] EtherType handling
- [ ] EAPOL parser
- [ ] EAPOL-Key fields
- [ ] Replay counter
- [ ] MIC / ACK / Install / Secure flags
- [ ] Handshake state machine

> EAPOL functionality is intended for authorized security research and analysis of networks you own or are explicitly permitted to test.

### Phase 4 — Storage

- [ ] PCAP
- [ ] CSV
- [ ] JSON
- [ ] Logs
- [ ] Configuration persistence
- [ ] Storage rotation

### Phase 5 — Web Interface

- [ ] Dashboard
- [ ] Networks
- [ ] Devices
- [ ] Statistics
- [ ] Captures
- [ ] Files
- [ ] Logs
- [ ] Configuration
- [ ] System information

### Phase 6 — TFT Interface

- [ ] Dashboard
- [ ] Scanner
- [ ] Networks
- [ ] Clients
- [ ] Statistics
- [ ] Captures
- [ ] Settings
- [ ] Mascot

### Phase 7 — Autonomous Behavior / AI

- [ ] Observation model
- [ ] Feature extraction
- [ ] Reward model
- [ ] Action space
- [ ] Embedded inference strategy
- [ ] Persistent model/state
- [ ] Mascot behavior integration

AI work starts only after the underlying telemetry and state model are stable.

---

## Repository Structure

The repository is being organized around responsibilities rather than individual features:

```text
WiFiCore/
├── README.md
├── LICENSE
├── platformio.ini
├── .gitignore
│
├── docs/
│   ├── ARCHITECTURE.md
│   ├── HARDWARE.md
│   ├── DEVELOPMENT.md
│   └── ROADMAP.md
│
├── include/
│   ├── config.h
│   ├── events.h
│   └── types.h
│
├── src/
│   ├── main.cpp
│   │
│   ├── core/
│   │   ├── RadioManager.cpp
│   │   ├── WiFiCapture.cpp
│   │   ├── ChannelManager.cpp
│   │   ├── FrameParser.cpp
│   │   ├── DeviceManager.cpp
│   │   ├── EventBus.cpp
│   │   └── Statistics.cpp
│   │
│   ├── storage/
│   │   ├── SDManager.cpp
│   │   ├── StorageTask.cpp
│   │   ├── PCAPWriter.cpp
│   │   ├── CSVWriter.cpp
│   │   └── JSONWriter.cpp
│   │
│   ├── display/
│   │   ├── TFTDisplay.cpp
│   │   └── Mascot.cpp
│   │
│   ├── web/
│   │   └── WebPortal.cpp
│   │
│   └── ai/
│       ├── Environment.cpp
│       ├── Observation.cpp
│       ├── Reward.cpp
│       └── Agent.cpp
│
└── data/
    └── .gitkeep
```

**Important:** this is the target architecture. The source tree will be migrated incrementally so that the current firmware is not destabilized by documentation/organization work.

---

## Design Principles

### 1. Capture must be fast

The Wi-Fi receive path should do the minimum possible work:

```text
ISR / Wi-Fi callback
        ↓
small frame metadata
        ↓
queue
        ↓
processing task
```

No display operations, filesystem writes or expensive parsing should happen inside the capture callback.

### 2. One owner for persistent storage

SD access should be centralized through a storage layer/task. Other modules publish data/events instead of directly writing to the card.

### 3. Interfaces consume state

The TFT and Web interface are consumers of the Core. They should not own Wi-Fi capture logic.

### 4. Event-driven communication

Modules communicate through typed events wherever practical instead of tightly coupling every subsystem to every other subsystem.

### 5. AI is a consumer of telemetry

The AI should not control raw capture. It receives a stable observation/state representation and produces high-level decisions for the Core.

---

## Development Environment

The project uses **PlatformIO + Arduino**. The repository currently contains a PlatformIO configuration and TFT_eSPI/Async Web dependencies from the existing implementation.

The hardware configuration is being migrated from the original ESP32/CYD setup to the ESP32-S3 N16R8 + 2.8" TFT target.

---

## Project Status

**Active development.**

The current priority is not adding more features. The priority is building a reliable embedded foundation capable of supporting the Wi-Fi Core, persistent storage, interfaces and eventually autonomous behavior.

---

## Disclaimer

This project is intended for **education, defensive security research and authorized wireless testing**. Only monitor or analyze networks and devices for which you have permission.

---

## Author

**Mauricio Portela**

Cybersecurity • Network Security • Wireless Security • Embedded Security

[GitHub](https://github.com/Mauricios73)

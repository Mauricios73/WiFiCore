# ESPGotchi / WiFiCore Roadmap

## 01 — Infrastructure

- [ ] EventBus robustness
- [ ] SD mutex/serialization
- [ ] Single StorageTask
- [ ] CaptureQueue
- [ ] Queue overflow handling
- [ ] Dropped-frame counters
- [ ] Heap/PSRAM monitoring
- [ ] Watchdog
- [ ] Global system state

## 02 — Wi-Fi Core

- [ ] RadioManager
- [ ] ChannelManager
- [ ] 802.11 parser
- [ ] Frame subtypes
- [ ] ToDS / FromDS
- [ ] QoS
- [ ] DeviceManager
- [ ] AP detection
- [ ] Client detection

## 03 — Protocol Analysis

- [ ] LLC/SNAP
- [ ] EtherType
- [ ] EAPOL
- [ ] EAPOL-Key
- [ ] Replay counter
- [ ] Key-information flags
- [ ] Handshake state machine

## 04 — Storage

- [ ] PCAP writer
- [ ] CSV writer
- [ ] JSON writer
- [ ] Structured logs
- [ ] Configuration persistence
- [ ] File rotation

## 05 — Web

- [ ] Dashboard
- [ ] Networks
- [ ] Devices
- [ ] Statistics
- [ ] Captures
- [ ] Files
- [ ] Logs
- [ ] Configuration
- [ ] System

## 06 — TFT

- [ ] 2.8" ILI9341 driver/configuration
- [ ] Dashboard
- [ ] Scanner
- [ ] Networks
- [ ] Clients
- [ ] Statistics
- [ ] Captures
- [ ] Settings
- [ ] Mascot

## 07 — Autonomous Behavior

- [ ] Observation model
- [ ] Feature extraction
- [ ] Reward model
- [ ] Action model
- [ ] Embedded inference
- [ ] Persistent AI state
- [ ] Mascot behavior

## 08 — Hardening

- [ ] Failure recovery
- [ ] Watchdog recovery
- [ ] SD corruption protection
- [ ] Memory stress tests
- [ ] Long-running capture tests
- [ ] Queue saturation tests
- [ ] Documentation of resource limits

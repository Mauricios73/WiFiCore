# Development Guide

## Toolchain

Recommended environment:

- VS Code
- PlatformIO
- Arduino framework
- ESP32-S3 development board

## Build

From the repository root:

```bash
pio run
```

Upload:

```bash
pio run -t upload
```

Serial monitor:

```bash
pio device monitor -b 115200
```

## Development Rules

### Keep the Core independent

New features should first be implemented as Core services/events before being added to TFT or Web.

### Avoid work in capture callbacks

Do not perform filesystem writes, display operations, logging-heavy work or dynamic allocations in the Wi-Fi callback.

### Measure before optimizing

For every infrastructure change, monitor:

- free heap
- minimum free heap
- PSRAM availability
- task stack high-water marks
- queue occupancy
- dropped frames
- CPU load
- watchdog resets

### Storage discipline

Do not allow multiple modules to write the SD card independently. Persistent writes belong to the storage subsystem.

### Interface discipline

The TFT and Web modules should read published state and issue commands/events. They should not reach into internal Wi-Fi driver state.

## Commit Style

Prefer small, descriptive commits:

```text
core: add capture queue overflow metrics
storage: serialize SD writes
parser: decode QoS data frames
tft: add network statistics screen
web: expose system telemetry
ai: add observation vector
```

## Current Priority

Infrastructure stability comes before new features.

1. EventBus
2. SD synchronization
3. StorageTask
4. CaptureQueue
5. overflow/dropped-frame metrics
6. heap monitoring
7. watchdog
8. global system state
9. Wi-Fi Core
10. storage formats
11. Web
12. TFT
13. AI

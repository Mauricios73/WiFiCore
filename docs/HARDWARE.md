# Hardware

## Current Target

### ESP32-S3 N16R8

The current ESPGotchi hardware target is an ESP32-S3 with:

- 16 MB flash
- 8 MB PSRAM
- integrated 2.4 GHz Wi-Fi
- dual-core Xtensa LX7 architecture
- Arduino / PlatformIO development

### Display

Target display:

- 2.8 inch TFT
- ILI9341 controller
- SPI interface
- 240 × 320 resolution

### Storage

The project uses an SD-card interface integrated with the target display/module where available.

The exact CS/SPI pin mapping is treated as board-specific configuration and should not be hard-coded into the Core modules.

## Hardware Abstraction

Hardware-specific definitions should be centralized in configuration files. Core modules should depend on interfaces/configuration rather than a particular display board.

## Removed Hardware

Audio is intentionally not part of the current ESPGotchi target. The mascot is visual-only through the TFT.

## Previous Hardware

The repository originally contained code for an ESP32/CYD-style board and TFT_eSPI configuration. That legacy implementation is being migrated toward the ESP32-S3 N16R8 target.

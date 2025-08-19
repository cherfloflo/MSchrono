# MSChrono

ESP32 + HUB75 RGB matrix (DMA) demo: header + seconds counter.

## Environments
- `esp32`: Arduino on ESP32 Dev Module.
- `native`: PC simulator (Python script in `tools/sim.py`).

## Build
- ESP32 build: use PlatformIO: `Run` or `pio run`.
- Native C++ sim: needs a C++ compiler (g++). Optional.

## Simulate (no hardware)
Run:

```
python tools/sim.py
```

This generates PPM images at project root (color bars + frames 0..5).

## Wiring
Defined via `build_flags` in `platformio.ini` (HUB75_* pins). Adjust to your board.

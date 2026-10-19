# Changelog

## 0.1.1 - 2026-10-19

- Include API documentation, validation notes and host tests in PlatformIO exports.
- Add explicit pending hardware-validation wording to the registry description.
- Document accepted Arduino registration and the full SuperJupiter package name.
- No sensor-monitor behaviour changes. Physical hardware validation remains pending.

## 0.1.0 - 2026-10-19

Initial public preview of SuperJupiter SensorQuality.

- Portable, allocation-free C++11 scalar sensor monitor.
- Flags for invalid numbers, range, rate, late observations, stale data and possibly stuck readings.
- Preserved raw values and capture timestamps; explicit invalid-configuration and timestamp errors.
- Synthetic Arduino examples, a host demonstration, tests and API documentation.
- MIT licence. Hardware validation pending.

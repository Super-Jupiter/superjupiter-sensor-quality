# Changelog

## 0.2.1 - 2026-10-19

- Explicit numeric-error flag and zero unavailable rate for reduced-precision rate overflow; raw readings retained.
- Added focused review regression tests and shared interoperability guidance. Hardware validation pending.


## 0.2.0 - 2026-10-19

- Breaking API convention: types, flags and helpers move to
  `superjupiter::sensor_quality`. Package/header names remain unchanged.
- Add migration and shared library conventions; no root compatibility aliases.
- Export `SuperJupiter::SensorQuality` for CMake subprojects and relocatable
  installed packages; developer tools/install rules are opt-in for subprojects.
- Switch registered Arduino lint to update mode and add release consistency,
  clean exported-package and downstream consumer checks.
- Add time/numeric/combined-flag boundaries and namespace isolation coverage.
- Reject incompatible GCC/Clang fast-math assumptions at compile time.
- Lead installation docs with registries and exact version pins.
- Physical hardware validation remains pending. Monitor semantics are unchanged.

## 0.1.2 - 2026-10-19

- Use absolute documentation/licence links so the README works on registry pages.
- Add the live PlatformIO installation link.
- No sensor-monitor or test changes. Physical hardware validation remains pending.

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

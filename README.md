# SuperJupiter SensorQuality

[![CI](https://github.com/Super-Jupiter/superjupiter-sensor-quality/actions/workflows/ci.yml/badge.svg)](https://github.com/Super-Jupiter/superjupiter-sensor-quality/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Release](https://img.shields.io/github/v/release/Super-Jupiter/superjupiter-sensor-quality?include_prereleases)](https://github.com/Super-Jupiter/superjupiter-sensor-quality/releases)

**Flag suspicious sensor readings without changing the raw data.**

A small, allocation-free C++11 library for Arduino, ESP32 and host applications.
Use one monitor per sensor channel to report invalid numbers, out-of-range values,
rapid changes, late samples, stale data and possibly stuck signals.

**Status: early release. Software tests and board compilation are automated;
physical hardware validation is pending.** A passing quality check is not proof
that a sensor is accurate. This library does not provide safety certification.

```text
Your sensor driver -> raw value + sample timestamp -> SensorQuality -> quality flags
                           |                              |
                           +---------- raw data ----------+-> your logger / application
```

## Why use it?

- Keep the original measurement, including invalid values, available for investigation.
- Catch stale data when a driver stops producing new measurements.
- Make quality problems explicit instead of quietly replacing or smoothing values.
- Use your existing sensor driver, storage and transport. No account, network or cloud required.
- No heap allocation, external dependencies, background task or hidden clock reads.

## Installation

### Arduino IDE

1. Download the source ZIP from [Releases](https://github.com/Super-Jupiter/superjupiter-sensor-quality/releases).
2. In Arduino IDE choose **Sketch > Include Library > Add .ZIP Library**.
3. Open **File > Examples > SuperJupiterSensorQuality > SyntheticTemperature**.
4. Select your board, upload and open Serial Monitor at **115200 baud**.

Library Manager registration is not yet complete. Use ZIP installation for now.

### PlatformIO

Pin the release explicitly in your project's `platformio.ini`:

```ini
lib_deps =
    https://github.com/Super-Jupiter/superjupiter-sensor-quality.git#v0.1.0
```

No PlatformIO Registry listing is claimed yet.

## Quick start

```cpp
#include <SuperJupiterSensorQuality.h>

superjupiter::Config makeConfig() {
  superjupiter::Config c;
  c.rangeEnabled = true;
  c.minimum = -40.0f;            // Example Celsius limits, not universal limits.
  c.maximum = 125.0f;
  c.expectedIntervalMs = 100;   // Expected observation interval, approximately 10 Hz.
  c.gapToleranceMs = 30;
  c.staleAfterMs = 750;
  c.maxRatePerSecond = 10.0f;   // Example limit in degrees C / second.
  c.stuckAfterMs = 5000;
  c.stuckTolerance = 0.02f;
  return c;
}

superjupiter::SensorQuality temperature(makeConfig());

void onFreshReading(float rawCelsius, uint32_t sampledAtMs) {
  const superjupiter::Result result = temperature.observe(rawCelsius, sampledAtMs);
  // Store result.value and result.flags together. The value is not corrected.
  if (result.has(superjupiter::OutOfRange)) {
    // Your application decides whether to log, alert or ignore this reading.
  }
}

void checkWhileWaiting(uint32_t nowMs) {
  const superjupiter::Result current = temperature.inspect(nowMs);
  if (current.has(superjupiter::Stale)) {
    // No sufficiently recent finite, in-range observation is available.
  }
}
```

**Call `observe()` only for a fresh measurement.** Resubmitting a cached value with
a new timestamp hides staleness. Reading a thermocouple interface at 10 Hz does
not prove that its converter supplies 10 fresh conversions per second.

## What the flags mean

| Flag | Meaning |
| --- | --- |
| `NoData` | No observation has been accepted since construction or reset. |
| `InvalidNumber` | The raw reading is NaN or positive/negative infinity. |
| `OutOfRange` | The reading is outside your inclusive minimum/maximum bounds. |
| `RateExceeded` | The absolute change per second exceeds your configured limit. |
| `Stuck` | Consecutive readings stayed within tolerance of a fixed anchor long enough to be suspicious. |
| `SampleGap` | The interval exceeds expected interval plus tolerance. It does not prove how many samples were lost. |
| `Stale` | No finite, in-range observation exists, or its age meets/exceeds the configured limit. |
| `InvalidTimestamp` | A timestamp is duplicate, backward or outside the supported comparison span. |
| `InvalidConfiguration` | Settings are inconsistent or contain unsupported values. |

Flags can occur together. A constant temperature can be entirely legitimate;
`Stuck` is a prompt to investigate, not a diagnosis. All configurable checks are
disabled by default. Invalid numbers and timestamp ordering are always checked.

See [API and exact semantics](docs/API.md) before using flags to drive decisions.

## Try it without hardware

On a machine with a C++11 compiler:

```sh
g++ -std=c++11 -Wall -Wextra -Werror -Isrc tools/replay.cpp -o replay
./replay
```

The synthetic example shows an excursion, NaN, out-of-range data, a possibly stuck
signal and stale data. There is no sensor, network or hidden telemetry.

```text
sampled_at_ms,raw_value,flags
0,20,healthy
100,20.1,healthy
200,80,rate_exceeded
300,20.1,rate_exceeded
400,nan,invalid_number
500,200,out_of_range
...
900,22,possibly_stuck
900,22,possibly_stuck|stale
```

The repeated `900` timestamp is intentional: `inspect()` does not invent a fresh sample.

## Testing and compatibility

The core uses standard C++11 with `<stdint.h>` and `<float.h>`. Arduino metadata
uses `architectures=*` to describe this portable source; that is not a claim that
every board has been physically tested.

CI runs host tests with GCC and Clang, including address/undefined-behaviour
sanitisers, and compiles both examples for ESP32, ESP32-S3, ESP32-C3 and Arduino Uno.
See the [actual CI results](https://github.com/Super-Jupiter/superjupiter-sensor-quality/actions)
and [validation record](docs/VALIDATION.md) for evidence and limitations.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

No persistence, smoothing, calibration, sensor-fault register decoding, automatic
retries or transport is included. Monitor calls must be serialised by the caller.

## Roadmap

- Validate the synthetic example on real ESP32 hardware.
- Add an accurately documented integration with an established sensor driver.
- Publish measured memory and timing results for exact board/compiler versions.
- Submit the reviewed release to Arduino Library Manager and PlatformIO Registry.

## Contributing and support

Bug reports and small, well-tested contributions are welcome. Include settings,
timestamps, expected behaviour and a minimal reproduction. Read
[CONTRIBUTING.md](CONTRIBUTING.md) and [SECURITY.md](SECURITY.md).

## About SuperJupiter

[SuperJupiter](https://www.superjupiter.net) builds custom software, connected
devices and operational platforms. This library is a standalone, free component
for the wider developer community, under the [MIT licence](LICENSE).

No SuperJupiter service is required to use it. Contact: **info@superjupiter.net**.

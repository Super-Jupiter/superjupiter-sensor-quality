// SPDX-License-Identifier: MIT
// Portable synthetic demonstration. Run with no hardware or network access.
#include "SuperJupiterSensorQuality.h"
#include <iostream>
#include <limits>

int main() {
  superjupiter::Config config;
  config.rangeEnabled = true;
  config.minimum = -40;
  config.maximum = 125;
  config.expectedIntervalMs = 100;
  config.gapToleranceMs = 20;
  config.maxRatePerSecond = 10;
  config.staleAfterMs = 500;
  config.stuckAfterMs = 300;
  superjupiter::SensorQuality quality(config);
  const float values[] = {20, 20.1f, 80, 20.1f,
    std::numeric_limits<float>::quiet_NaN(), 200, 22, 22, 22, 22};
  const superjupiter::Flag flags[] = {
    superjupiter::NoData, superjupiter::InvalidNumber, superjupiter::OutOfRange,
    superjupiter::RateExceeded, superjupiter::Stuck, superjupiter::SampleGap,
    superjupiter::Stale, superjupiter::InvalidTimestamp, superjupiter::InvalidConfiguration};
  std::cout << "Synthetic data only\nsampled_at_ms,raw_value,flags\n";
  for (uint32_t i = 0; i < 12; ++i) {
    const superjupiter::Result r = i < 10 ? quality.observe(values[i], i * 100) :
      quality.inspect(900 + (i - 9) * 500);
    std::cout << r.sampledAtMs << ',' << r.value << ',';
    if (r.healthy()) std::cout << "healthy";
    bool separator = false;
    for (unsigned j = 0; j < sizeof(flags) / sizeof(flags[0]); ++j) {
      if (r.has(flags[j])) {
        if (separator) std::cout << '|';
        std::cout << superjupiter::flagName(flags[j]);
        separator = true;
      }
    }
    std::cout << '\n';
  }
}

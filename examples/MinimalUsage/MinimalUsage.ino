// SPDX-License-Identifier: MIT
// Synthetic values. Replace the value source only after configuring limits
// appropriate for your sensor. This is not a thermocouple driver.
#include <SuperJupiterSensorQuality.h>

superjupiter::Config settings() {
  superjupiter::Config c;
  c.rangeEnabled = true;
  c.minimum = -40;
  c.maximum = 125;
  c.staleAfterMs = 1000;
  return c;
}

superjupiter::SensorQuality monitor(settings());
uint32_t lastSampleAt = 0;

void setup() { Serial.begin(115200); }

void loop() {
  const uint32_t now = millis();
  if (now - lastSampleAt >= 100) {
    lastSampleAt = now;
    const float syntheticTemperature = 20.0f;
    const superjupiter::Result r = monitor.observe(syntheticTemperature, now);
    Serial.print(r.value);
    Serial.print(" quality_flags=");
    Serial.println(r.flags);
  }
  // In a real application call this even when the driver has no fresh data.
  const superjupiter::Result current = monitor.inspect(now);
  (void)current;
}

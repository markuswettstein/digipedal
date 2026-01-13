#include "digipot.h"
#include "pins.h"

#include <Arduino.h>
#include <Wire.h>

namespace digipot {

static constexpr uint8_t CMD_WIPER0_WRITE = 0x00; // family command/address for volatile Wiper0 write

static uint8_t clamp_u8(uint16_t v) {
  return (v > 255) ? 255 : static_cast<uint8_t>(v);
}

static uint8_t pct_to_u8(float pct) {
  if (pct <= 0.0f) return 0;
  if (pct >= 100.0f) return 255;
  // round to nearest
  float scaled = (pct * 255.0f) / 100.0f;
  return static_cast<uint8_t>(scaled + 0.5f);
}

static bool write_wiper0(uint8_t i2c_addr, uint8_t value) {
  Wire.beginTransmission(i2c_addr);
  Wire.write(CMD_WIPER0_WRITE);
  Wire.write(value);
  return (Wire.endTransmission() == 0);
}

void init() {
  // nothing yet; placeholder for future (e.g. default values, device check)
}

bool set_drive(uint8_t value) {
  return write_wiper0(I2C_ADDR_POT_DRIVE, value);
}

bool set_tone(uint8_t value) {
  return write_wiper0(I2C_ADDR_POT_TONE, value);
}

bool set_drive_pct(float pct) {
  return set_drive(pct_to_u8(pct));
}

bool set_tone_pct(float pct) {
  return set_tone(pct_to_u8(pct));
}

} // namespace digipot

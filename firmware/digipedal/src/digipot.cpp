#include "digipot.h"
#include "pins.h"

#include <Wire.h>

namespace digipot {

static constexpr uint8_t CMD_WIPER0_WRITE = 0x00; // Write Data, Wiper0
static constexpr uint8_t CMD_WIPER0_READ  = 0x0C; // Read Data,  Wiper0
static constexpr uint8_t WIPER_MAX        = 128;  // MCP45HV31T is 7-bit (129 positions: 0..128)

static bool write_wiper0(uint8_t i2c_addr, uint8_t value) {
  Wire.beginTransmission(i2c_addr);
  Wire.write(CMD_WIPER0_WRITE);
  Wire.write(value);
  return (Wire.endTransmission() == 0);
}

static bool read_wiper0(uint8_t i2c_addr, uint8_t &value_out) {
  // Send read command
  Wire.beginTransmission(i2c_addr);
  Wire.write(CMD_WIPER0_READ);
  if (Wire.endTransmission(false) != 0) return false; // repeated start

  // Read 2 bytes (most MCP45xx return 2: status/high bits + data byte)
  int n = Wire.requestFrom((int)i2c_addr, 2);
  if (n < 2) return false;

  uint8_t msb = (uint8_t)Wire.read();
  uint8_t lsb = (uint8_t)Wire.read();

  // MSB carries status bits: bit1 = SHDN (shutdown), bit0 = D8 (unused on 7-bit parts)
  if (msb & 0x02) return false;  // pot is in shutdown state
  value_out = lsb;
  return true;
}

void init() {}

bool set_drive(uint8_t value) { return write_wiper0(I2C_ADDR_POT_DRIVE, value); }
bool set_tone(uint8_t value)  { return write_wiper0(I2C_ADDR_POT_TONE,  value); }

bool set_drive_pct(float pct) {
  if (pct <= 0.0f) return set_drive(0);
  if (pct >= 100.0f) return set_drive(WIPER_MAX);
  return set_drive((uint8_t)((pct * WIPER_MAX / 100.0f) + 0.5f));
}

bool set_tone_pct(float pct) {
  if (pct <= 0.0f) return set_tone(0);
  if (pct >= 100.0f) return set_tone(WIPER_MAX);
  return set_tone((uint8_t)((pct * WIPER_MAX / 100.0f) + 0.5f));
}

bool read_drive(uint8_t &value) { return read_wiper0(I2C_ADDR_POT_DRIVE, value); }
bool read_tone(uint8_t &value)  { return read_wiper0(I2C_ADDR_POT_TONE,  value); }

} // namespace digipot

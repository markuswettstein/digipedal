#pragma once
#include <stdint.h>

namespace digipot {

// TO be called once after Wire.begin()
void init();

// Raw wiper control (0..128, device max for 7-bit MCP45HV31T). Returns true on I2C success.
bool set_drive(uint8_t value);
bool set_tone(uint8_t value);

// Convenience mapping (0..100%). Returns true on I2C success.
bool set_drive_pct(float pct);
bool set_tone_pct(float pct);

bool read_drive(uint8_t &value);
bool read_tone(uint8_t &value);


} // namespace digipot

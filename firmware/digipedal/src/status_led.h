#pragma once
#include <stdint.h>

namespace status_led {

void init();
void off();

// Very short flash to indicate activity (MIDI, I2C, etc.)
void activity();

// Solid colors for states
void ok();
void error();

} // namespace status_led

#pragma once
#include <stdint.h>
#include "midi_parser.h"

namespace midi_handler {

struct Config {
  uint8_t channel   = 0;   // MIDI channel 1-16; 0 = respond to all channels
  uint8_t cc_drive  = 16;  // General Purpose Controller 1
  uint8_t cc_tone   = 17;  // General Purpose Controller 2
  uint8_t cc_bypass = 64;  // Sustain pedal: value >= 64 → engaged, < 64 → bypassed
};

void init(const Config& cfg);
void handle(const midi::Message& m);

} // namespace midi_handler

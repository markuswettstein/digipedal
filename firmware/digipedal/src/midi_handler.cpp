#include "midi_handler.h"
#include "digipot.h"
#include "relay.h"

namespace midi_handler {

static Config active_cfg;

void init(const Config& cfg) {
  active_cfg = cfg;
}

void handle(const midi::Message& m) {
  if (active_cfg.channel != 0) {
    uint8_t msg_ch = (m.status & 0x0F) + 1;
    if (msg_ch != active_cfg.channel) return;
  }

  uint8_t type = m.status & 0xF0;

  if (type == 0xB0) {  // Control Change
    float pct = m.data2 * (100.0f / 127.0f);
    if (m.data1 == active_cfg.cc_drive)  digipot::set_drive_pct(pct);
    if (m.data1 == active_cfg.cc_tone)   digipot::set_tone_pct(pct);
    if (m.data1 == active_cfg.cc_bypass) relay::set(m.data2 >= 64);
  }
}

} // namespace midi_handler

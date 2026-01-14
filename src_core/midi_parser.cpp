#include "midi_parser.h"

namespace midi {

static bool is_realtime(uint8_t b) {
  return b >= 0xF8; // 0xF8..0xFF can appear anywhere
}

static bool is_status(uint8_t b) {
  return (b & 0x80) != 0;
}

static uint8_t channel_msg_len(uint8_t status) {
  // status high nibble
  switch (status & 0xF0) {
    case 0xC0: // Program Change
    case 0xD0: // Channel Pressure
      return 2;
    case 0x80: // Note Off
    case 0x90: // Note On
    case 0xA0: // Poly Pressure
    case 0xB0: // Control Change
    case 0xE0: // Pitch Bend
      return 3;
    default:
      return 0;
  }
}

std::optional<Message> Parser::feed(uint8_t b) {
  // Realtime can be interleaved anywhere; emit immediately.
  if (is_realtime(b)) {
    return Message{b, 0, 0, 1};
  }

  if (is_status(b)) {
    // System Common / SysEx start/stop: ignore for now (could be extended later)
    if (b >= 0xF0) {
      // Reset any partial channel message collection
      needed_ = 0;
      have_ = 0;
      return std::nullopt;
    }

    // Channel Voice status
    running_status_ = b;
    needed_ = channel_msg_len(b);
    have_ = 0;
    return std::nullopt;
  }

  // Data byte
  if (needed_ == 0) {
    // Running status case: if we have a running status, reconstruct message.
    if (running_status_ != 0) {
      needed_ = channel_msg_len(running_status_);
      have_ = 0;
    } else {
      return std::nullopt;
    }
  }

  // Collect data bytes
  if (have_ < 2) buf_[have_] = b;
  have_++;

  if (needed_ == 2 && have_ >= 1) {
    Message m;
    m.status = running_status_;
    m.data1 = buf_[0];
    m.data2 = 0;
    m.len = 2;
    needed_ = 0;
    have_ = 0;
    return m;
  }

  if (needed_ == 3 && have_ >= 2) {
    Message m;
    m.status = running_status_;
    m.data1 = buf_[0];
    m.data2 = buf_[1];
    m.len = 3;
    needed_ = 0;
    have_ = 0;
    return m;
  }

  return std::nullopt;
}

void Parser::reset() {
  running_status_ = 0;
  needed_ = 0;
  have_ = 0;
  buf_[0] = buf_[1] = 0;
}

} // namespace midi

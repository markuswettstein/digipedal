#pragma once
#include <cstdint>
#include <optional>

namespace midi {

struct Message {
  uint8_t status = 0;
  uint8_t data1  = 0;
  uint8_t data2  = 0;
  uint8_t len    = 0;   // 1, 2, or 3
};

// Minimal byte-stream parser for Channel Voice + System Realtime.
// - Supports running status for channel voice messages
// - Emits realtime (0xF8..0xFF) as single-byte messages
// - Ignores SysEx/System Common for now (returns no messages for them)
class Parser {
public:
  std::optional<Message> feed(uint8_t b);
  void reset();

private:
  uint8_t running_status_ = 0;
  uint8_t needed_ = 0;
  uint8_t buf_[2] = {0, 0};
  uint8_t have_ = 0;
};

} // namespace midi

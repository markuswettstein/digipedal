#define CATCH_CONFIG_MAIN
#include "third_party/catch2/catch.hpp"

#include "../src_core/midi_parser.h"
#include <vector>

static std::vector<midi::Message> feed_all(const std::vector<uint8_t>& bytes) {
  midi::Parser p;
  std::vector<midi::Message> out;
  for (auto b : bytes) {
    if (auto m = p.feed(b)) out.push_back(*m);
  }
  return out;
}

TEST_CASE("Running status for CC produces multiple messages") {
  // B0 10 01, then running status continues: 10 02, 10 03
  auto msgs = feed_all({0xB0, 0x10, 0x01, 0x10, 0x02, 0x10, 0x03});
  REQUIRE(msgs.size() == 3);

  REQUIRE(msgs[0].status == 0xB0);
  REQUIRE(msgs[0].data1  == 0x10);
  REQUIRE(msgs[0].data2  == 0x01);
  REQUIRE(msgs[0].len    == 3);

  REQUIRE(msgs[1].status == 0xB0);
  REQUIRE(msgs[1].data1  == 0x10);
  REQUIRE(msgs[1].data2  == 0x02);
  REQUIRE(msgs[1].len    == 3);

  REQUIRE(msgs[2].status == 0xB0);
  REQUIRE(msgs[2].data1  == 0x10);
  REQUIRE(msgs[2].data2  == 0x03);
  REQUIRE(msgs[2].len    == 3);
}

TEST_CASE("Realtime bytes are emitted even when interleaved") {
  auto msgs = feed_all({0xF8, 0x90, 0x3C, 0x7F, 0xF8, 0x80, 0x3C, 0x00});
  REQUIRE(msgs.size() == 4);

  REQUIRE(msgs[0].status == 0xF8);
  REQUIRE(msgs[0].len == 1);

  REQUIRE(msgs[1].status == 0x90);
  REQUIRE(msgs[1].data1 == 0x3C);
  REQUIRE(msgs[1].data2 == 0x7F);
  REQUIRE(msgs[1].len == 3);

  REQUIRE(msgs[2].status == 0xF8);
  REQUIRE(msgs[2].len == 1);

  REQUIRE(msgs[3].status == 0x80);
  REQUIRE(msgs[3].data1 == 0x3C);
  REQUIRE(msgs[3].data2 == 0x00);
  REQUIRE(msgs[3].len == 3);
}

TEST_CASE("Program change is 2 bytes") {
  auto msgs = feed_all({0xC2, 0x05});
  REQUIRE(msgs.size() == 1);
  REQUIRE(msgs[0].status == 0xC2);
  REQUIRE(msgs[0].data1 == 0x05);
  REQUIRE(msgs[0].len == 2);
}

#include "bringup.h"
#include "pins.h"
#include "status_led.h"
#include "digipot.h"
#include "relay.h"
#include "midi_handler.h"
#include "midi_parser.h"

#include <Arduino.h>
#include <Wire.h>

static constexpr bool  RELAY_SELFTEST_ON_BOOT = true;
static constexpr uint16_t RELAY_PULSE_MS      = 600;

// Send 0xFE (Active Sensing) on TX to verify the UART during bringup
static constexpr bool  MIDI_TX_TEST           = false;
static constexpr uint16_t MIDI_TX_PERIOD_MS   = 1000;

static midi::Parser rx_parser;

static void maybe_print_banner() {
  static bool printed = false;
  if (!printed && Serial) {
    Serial.println("digipedal bring-up");
    Serial.print("standalone="); Serial.println(STANDALONE_MODE ? "true" : "false");
    printed = true;
  }
}

void bringup_init() {
  status_led::init();
  Serial.begin(115200);

  // Wait up to 1 s for USB Serial, then proceed without it
  uint32_t t = millis();
  while (!Serial && (millis() - t < 1000)) {}

  if (!STANDALONE_MODE) {
    relay::init();

    Wire.begin();
    Wire.setClock(I2C_FREQ);

    digipot::init();
    digipot::set_drive(0);
    digipot::set_tone(0);

    // I2C scan
    if (Serial) Serial.println("I2C scan:");
    for (uint8_t addr = 1; addr < 127; addr++) {
      Wire.beginTransmission(addr);
      if (Wire.endTransmission() == 0) {
        if (Serial) {
          Serial.print("  found 0x");
          if (addr < 16) Serial.print('0');
          Serial.println(addr, HEX);
        }
      }
    }

    // Digipot write/read verification
    uint8_t r = 0;
    digipot::set_drive(10);
    digipot::set_tone(100);
    delay(10);

    bool ok_drive = digipot::read_drive(r);
    if (Serial) {
      Serial.print("DRIVE read: ");
      Serial.print(ok_drive ? "ok " : "fail ");
      if (ok_drive) Serial.println(r);
      else          Serial.println();
    }
    if (!ok_drive) status_led::error();

    bool ok_tone = digipot::read_tone(r);
    if (Serial) {
      Serial.print("TONE  read: ");
      Serial.print(ok_tone ? "ok " : "fail ");
      if (ok_tone) Serial.println(r);
      else         Serial.println();
    }
    if (!ok_tone) status_led::error();

    // Reset pots to safe starting position
    digipot::set_drive(0);
    digipot::set_tone(0);

    Serial1.begin(MIDI_BAUD);

    if (MIDI_TX_TEST) {
      Serial1.write(0xFE);
      if (Serial) Serial.println("TX 0xFE");
    }

    if (RELAY_SELFTEST_ON_BOOT) {
      relay::set(true);
      delay(RELAY_PULSE_MS);
      relay::set(false);
      if (Serial) Serial.println("relay selftest pulse");
    }

    midi_handler::init(midi_handler::Config{});

    status_led::ok();
  }
}

void bringup_loop() {
  maybe_print_banner();

  if (STANDALONE_MODE) {
    static uint32_t t0 = 0;
    static uint8_t phase = 0;
    if (millis() - t0 > 700) {
      t0 = millis();
      phase = (phase + 1) % 4;
      switch (phase) {
        case 0: status_led::off();      break;
        case 1: status_led::ok();       break;
        case 2: status_led::activity(); break;
        case 3: status_led::error();    break;
      }
    }
    return;
  }

  if (MIDI_TX_TEST) {
    static uint32_t last_tx = 0;
    if (millis() - last_tx > MIDI_TX_PERIOD_MS) {
      last_tx = millis();
      Serial1.write(0xFE);
      if (Serial) Serial.println("TX 0xFE");
    }
  }

  status_led::tick();

  while (Serial1.available()) {
    uint8_t b = static_cast<uint8_t>(Serial1.read());
    auto msg = rx_parser.feed(b);
    if (!msg) continue;

    status_led::activity();
    midi_handler::handle(*msg);

    if (Serial) {
      Serial.print("MIDI 0x"); Serial.print(msg->status, HEX);
      Serial.print(" 0x");     Serial.print(msg->data1,  HEX);
      if (msg->len > 2) {
        Serial.print(" 0x");   Serial.print(msg->data2,  HEX);
      }
      Serial.println();
    }
  }
}

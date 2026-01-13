#include "bringup.h"
#include "pins.h"
#include "status_led.h"
#include "digipot.h"

#include <Arduino.h>
#include <Wire.h>


static uint32_t last_activity_ms = 0;

static void maybe_print_banner() {
  static bool printed = false;
  if (!printed && Serial) {
    Serial.println("digipedal bring-up");
    Serial.print("standalone="); Serial.println(STANDALONE_MODE ? "true" : "false");
    printed = true;
  }
}

void bringup_init() {
  status_led::init();     // turn LED off ASAP after reset
  Serial.begin(115200);   // non-blocking

  if (!STANDALONE_MODE) {
    pinMode(COIL_PIN, OUTPUT);
    digitalWrite(COIL_PIN, LOW);

    Wire.begin();
    Wire.setClock(I2C_FREQ);

	digipot::init();
	digipot::set_drive(0);
	digipot::set_tone(0);

    Serial1.begin(MIDI_BAUD);
  }
}

void bringup_loop() {
  maybe_print_banner();

  // Standalone: LED self-test so you can confirm control without MIDI/wiring
  if (STANDALONE_MODE) {
    static uint32_t t0 = 0;
    static uint8_t phase = 0;

    if (millis() - t0 > 700) {
      t0 = millis();
      phase = (phase + 1) % 4;

      switch (phase) {
        case 0: status_led::off();    break;
        case 1: status_led::ok();     break;      // dim blue
        case 2: status_led::activity(); break;     // dim green pulse
        case 3: status_led::error();  break;      // dim red
      }
    }
    return;
  }

  // Non-standalone: activity pulse timeout
  if (last_activity_ms && (millis() - last_activity_ms > 40)) {
    status_led::off();
    last_activity_ms = 0;
  }

  // MIDI bytes -> USB debug + activity LED
  while (Serial1.available()) {
    uint8_t b = static_cast<uint8_t>(Serial1.read());
    status_led::activity();
    last_activity_ms = millis();

    if (Serial) {
      Serial.print("0x");
      if (b < 16) Serial.print('0');
      Serial.print(b, HEX);
      Serial.print(' ');
    }
  }
}

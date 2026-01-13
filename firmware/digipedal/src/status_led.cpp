#include "status_led.h"
#include "pins.h"

#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>

namespace status_led {

static Adafruit_DotStar led(1, DOTSTAR_DATA_PIN, DOTSTAR_CLOCK_PIN, DOTSTAR_BRG);
static uint32_t last_activity_ms = 0;

// Keep it dim by default (0..255)
static constexpr uint8_t BRIGHTNESS_DIM = 20;

static void show_color(uint32_t c, uint8_t brightness) {
  led.setBrightness(brightness);
  led.setPixelColor(0, c);
  led.show();
}

void init() {
  led.begin();
  off();
}

void off() {
  show_color(0x000000, 0);
}

void activity() {
  // dim green pulse; caller can call repeatedly
  show_color(0x002000, BRIGHTNESS_DIM);
  last_activity_ms = millis();
}

void ok() {
  show_color(0x000020, BRIGHTNESS_DIM); // dim blue
}

void error() {
  show_color(0x200000, BRIGHTNESS_DIM); // dim red
}

// Optional helper
static void maybe_decay_activity() {
  if (last_activity_ms && (millis() - last_activity_ms > 40)) {
    off();
    last_activity_ms = 0;
  }
}



} // namespace status_led

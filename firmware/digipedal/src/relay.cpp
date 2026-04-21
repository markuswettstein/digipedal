#include "relay.h"
#include "pins.h"

#include <Arduino.h>

namespace relay {

static constexpr uint32_t DEBOUNCE_MS = 50;

static bool current_state  = false;
static uint32_t last_toggle_ms = 0;

void init() {
  pinMode(COIL_PIN, OUTPUT);
  digitalWrite(COIL_PIN, LOW);
  current_state  = false;
  last_toggle_ms = 0;
}

void set(bool engaged) {
  if (engaged == current_state) return;
  if (last_toggle_ms && (millis() - last_toggle_ms < DEBOUNCE_MS)) return;
  current_state  = engaged;
  last_toggle_ms = millis();
  digitalWrite(COIL_PIN, engaged ? HIGH : LOW);
}

bool state() { return current_state; }

} // namespace relay

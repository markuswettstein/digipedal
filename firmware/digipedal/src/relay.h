#pragma once
#include <stdbool.h>

namespace relay {

void init();

// Set the relay state. Calls within DEBOUNCE_MS of the last toggle are ignored.
void set(bool engaged);

bool state();

} // namespace relay

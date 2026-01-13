#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...


#define NUMPIXELS 1 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN   7
#define CLOCKPIN   8

Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);




void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off 
}
void loop() {}
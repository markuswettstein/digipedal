#include "bringup.h"
#include "pins.h"

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_DotStar.h>

static Adafruit_DotStar statusLED(
  1,
  DOTSTAR_DATA_PIN,
  DOTSTAR_CLOCK_PIN,
  DOTSTAR_BRG
);

static uint32_t lastActivityMs = 0;

static void led_off() {
  statusLED.setBrightness(0);
  statusLED.setPixelColor(0, 0);
  statusLED.show();
}

static void led_activity() {
  statusLED.setBrightness(2);
  statusLED.setPixelColor(0, 0x002000);
  statusLED.show();
  lastActivityMs = millis();
}

void bringup_init() {
  Serial.begin(115200);

  statusLED.begin();
  led_off();

  if (!STANDALONE_MODE) {
  	pinMode(COIL_PIN, OUTPUT);
  	digitalWrite(COIL_PIN, LOW);

  	Wire.begin();
  	Wire.setClock(I2C_FREQ);

  	Serial1.begin(MIDI_BAUD);
  } else {}
  
  delay(2000);
  Serial.println("digipedal bring-up");
}

void bringup_loop() {
  while (Serial1.available()) {
    uint8_t b = Serial1.read();
    led_activity();

    Serial.print("0x");
    if (b < 16) Serial.print('0');
    Serial.print(b, HEX);
    Serial.print(' ');
  }
}

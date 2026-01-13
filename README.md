# digipedal

Firmware for a MIDI-controlled analog guitar pedal (Trinket M0 / SAMD21).

## Build / Upload
- `make build`
- `make upload PORT=/dev/cu.usbmodemXXXX`

## Notes
- USB stack: Arduino (for reliable Serial debug)
- Status LED: onboard DotStar (data=7, clock=8)

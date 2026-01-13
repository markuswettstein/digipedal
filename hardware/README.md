# digipedal hardware

This folder contains the hardware design for the digipedal project.

## easyeda/
Editable EasyEDA design sources (schematic + PCB).

## exports/
Generated, human-readable artifacts:
- schematic PDFs
- PCB PDFs
- BOM CSV

## Notes
- MCU: Trinket M0 (SAMD21)
- MIDI IN: opto-isolated
- I2C devices used for analog control
- Status LED: onboard DotStar (data=7, clock=8)

Firmware relies on these pin assignments.

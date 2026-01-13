FQBN=adafruit:samd:adafruit_trinket_m0
PORT?=/dev/cu.usbmodem1101
SKETCH=firmware/digipedal

build:
	arduino-cli compile --fqbn $(FQBN) $(SKETCH)

upload:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(SKETCH)

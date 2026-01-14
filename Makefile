FQBN=adafruit:samd:adafruit_trinket_m0
PORT?=/dev/cu.usbmodem1101
SKETCH=firmware/digipedal
CXX := c++
CXXFLAGS := -std=gnu++17 -O2 -Wall -Wextra

clean:
	rm -rf build
build:
	arduino-cli compile --fqbn $(FQBN) $(SKETCH)

upload:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) $(SKETCH)

test:
	mkdir -p build
	$(CXX) $(CXXFLAGS) -Itests -Isrc_core \
	  src_core/midi_parser.cpp tests/test_midi_parser.cpp \
	  -o build/tests_midi_parser
	./build/tests_midi_parser

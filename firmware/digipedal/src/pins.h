#pragma once
#include <stdint.h>
#define BOARD_TRINKET_M0
constexpr bool STANDALONE_MODE = true;

// Status LED (onboard DotStar)
constexpr uint8_t DOTSTAR_DATA_PIN  = 7;
constexpr uint8_t DOTSTAR_CLOCK_PIN = 8;

// MIDI (DIN over UART)
constexpr uint32_t MIDI_BAUD = 31250;
constexpr uint8_t MIDI_RX_PIN = 3;
constexpr uint8_t MIDI_TX_PIN = 4;

// I2C
constexpr uint32_t I2C_FREQ = 100000;
constexpr uint8_t I2C_SDA_PIN = 0;
constexpr uint8_t I2C_SCL_PIN = 2;
constexpr uint8_t I2C_ADDR_POT_DRIVE = 0x3C;
constexpr uint8_t I2C_ADDR_POT_TONE  = 0x3D;


// Actuator / control
constexpr uint8_t COIL_PIN = 1;

// Misc
constexpr uint8_t LED_STATUS_PIN = 13;

#include <Arduino.h>

#ifndef Defines_h
#define Defines_h

// Pin definitions
#define BUZZER_PIN 4 //
#define MIXER_RELAY_PIN 26
#define SOIL_DISPENSER_RELAY_PIN 27

#define SENSOR_LOWER_LIMIT_PIN 32
#define SENSOR_UPPER_LIMIT_PIN 33

// USING HARDWARE SPI FOR MAX31865, SO ONLY NEED TO DEFINE THE CS PIN
#define MAX31865_CS_PIN 5

// SOIL MOISTURE SENSOR
#define MOISTURE_SENSOR_ANALOG_PIN 39

// MCP23017 IO expander mapping used for the machine I/O board.
// These are logical expander pins 0..15, not ESP32 GPIO numbers.
#define MCP_MIXER_RELAY_PIN 0
#define MCP_SOIL_DISPENSER_RELAY_PIN 1
#define MCP_SENSOR_LOWER_LIMIT_PIN 2
#define MCP_SENSOR_UPPER_LIMIT_PIN 3

#define MCP_INPUTS_ADDRESS 0x20
#define MCP_OUTPUTS_ADDRESS 0x21
#define MCP_INPUTS_INTERRUPT_PIN 34
#define MCP_OUTPUTS_INTERRUPT_PIN 35

#endif // Defines_h
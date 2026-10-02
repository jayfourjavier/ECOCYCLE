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
// GPIO15 is ADC2, which is blocked while Wi-Fi is active on ESP32.
// Use an ADC1-capable pin instead (e.g. GPIO35) so analog reads work with Wi-Fi on.
#define MOISTURE_SENSOR_ANALOG_PIN 35

#endif // Defines_h
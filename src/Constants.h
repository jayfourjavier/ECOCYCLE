#ifndef CONSTANTS_H
#define CONSTANTS_H

// Define the target weights for soil and food waste in grams per batch
#define TARGET_SOIL_WT_GRAMS 950
#define TARGET_FOOD_WASTE_WT_GRAMS 50

//
#define MIXING_DURATION_SECONDS 10

#define RECORD_TIMESTAMP_START_EPOCH 1788220800UL // 2026-09-01 00:00:00 UTC
#define RECORD_TIMESTAMP_END_EPOCH 1819756800UL   // 2027-09-01 00:00:00 UTC
#define RECORD_TEMPERATURE_MIN 0.0f
#define RECORD_TEMPERATURE_MAX 100.0f
#define RECORD_MOISTURE_MIN 0.0f
#define RECORD_MOISTURE_MAX 100.0f

#define FIREBASE_ACTIVITY_PATH "/activity"

#define RREF 430.0     // Reference resistor value in ohms
#define RNOMINAL 100.0 // Nominal resistance of PT100 at 0°C in ohms

#endif // CONSTANTS_H
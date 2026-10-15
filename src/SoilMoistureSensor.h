#ifndef SOIL_MOISTURE_SENSOR_H
#define SOIL_MOISTURE_SENSOR_H

#include <Arduino.h>

class SoilMoistureSensor
{
public:
    /*
     * @param pin Analog pin connected to sensor
     * @param zero Reading when dry (higher value)
     * @param hundred Reading when fully wet (lower value)
     * @param samples Number of samples to average for stable reading
     */
    SoilMoistureSensor(int pin, int zero, int hundred, int samples = 50)
        : sensorPin(pin), dryValue(zero), wetValue(hundred), sampleCount(samples)
    {
        pinMode(sensorPin, INPUT);
    }

    /*
     * @brief Reads raw analog value multiple times and returns average
     * @return Average raw analog value (0–1023)
     */
    unsigned int readRaw()
    {
        unsigned long sum = 0;
        for (int i = 0; i < sampleCount; i++)
        {
            sum += analogRead(sensorPin);
            delay(50);
        }
        rawMoisture = sum / sampleCount;
        return rawMoisture;
    }

    /*
     * @brief Reads moisture percentage based on calibration values
     * @return Moisture percentage (0–100)
     */
    unsigned int readMoisture()
    {
        rawMoisture = readRaw();

        if (rawMoisture >= dryValue)
        {
            moisture = 0;
        }
        else if (rawMoisture <= wetValue)
        {
            moisture = 100;
        }
        else
        {
            moisture = map(rawMoisture, dryValue, wetValue, 0, 100);
        }

        moisture = constrain(moisture, 0, 100);
        return moisture;
    }

    unsigned int getLastRaw() const { return rawMoisture; }
    unsigned int getLastMoisture() const { return moisture; }

private:
    int sensorPin;
    int dryValue;
    int wetValue;
    unsigned int rawMoisture = 0;
    unsigned int moisture = 0;
    int sampleCount = 50;
};

#endif

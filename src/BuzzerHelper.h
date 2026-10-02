#pragma once

#include <Arduino.h>

/**
 * @brief Non-blocking buzzer helper for short alarm tones.
 *
 * The buzzer does not block the program loop. Instead, it schedules a tone for
 * a fixed duration and stops automatically using millis().
 */
class BuzzerHelper
{
public:
    /**
     * @brief Creates a buzzer helper for the given GPIO pin.
     * @param pin Arduino pin connected to the buzzer.
     */
    explicit BuzzerHelper(uint8_t pin = 25)
        : buzzerPin_(pin), startMs_(0), durationMs_(0), active_(false)
    {
    }

    /**
     * @brief Initializes the buzzer pin.
     */
    void begin()
    {
        pinMode(buzzerPin_, OUTPUT);
        noTone(buzzerPin_);
        active_ = false;
    }

    /**
     * @brief Starts a 1 second failed-attempt buzzer pulse.
     */
    void failedAttempt()
    {
        startTone(1000UL);
    }

    /**
     * @brief Starts a 3 second failed-write buzzer pulse.
     */
    void failedWrite()
    {
        startTone(3000UL);
    }

    /**
     * @brief Updates the active buzzer state without blocking.
     *
     * This should be called from the main loop.
     */
    void update()
    {
        if (!active_)
            return;

        if (millis() - startMs_ >= durationMs_)
        {
            noTone(buzzerPin_);
            active_ = false;
        }
    }

private:
    void startTone(uint32_t durationMs)
    {
        startMs_ = millis();
        durationMs_ = durationMs;
        active_ = true;
        tone(buzzerPin_, 2000);
    }

    uint8_t buzzerPin_;
    uint32_t startMs_;
    uint32_t durationMs_;
    bool active_;
};

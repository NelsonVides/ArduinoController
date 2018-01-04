/*
 * Button.h
 * Created: 18/11/2014 19:33:02
 *  Author: Richard
 */ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Forked by Nelson Vides to implement ANALOG_PINS
 * and modern C++ features
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef BUTTONEVENTCALLBACK_H
#define BUTTONEVENTCALLBACK_H

#include <inttypes.h>
#include <Arduino.h>
#include "./Button.h"

class ButtonEventCallback {
public:
    ButtonEventCallback();

    EventType getType() const;

    void setType(EventType type);
    void setDelay(uint16_t delay);
    void setMaxDelay(uint16_t max_delay);
    void setRepetitionPeriod(uint16_t repeat_period);
    void setCallback(ButtonOnEventCallback callback);
    void setRepeatingCallback(ButtonOnEventRepeatCallback callback_repeating);

    void executeCallbackIfTime(uint16_t elapsedTime, boolean release_event, Button& btn);
    void reset();

private:
    EventType _type = EventType::evtUninitialised;
    uint16_t _delay = 0;
    uint16_t _max_delay = 0;
    uint16_t _repeat_period = 0;
    uint8_t _execution_count= 1;

    void calculateNextExecutionTime();

    // Keep track of when the callback should be executed
    uint16_t _next_execution_time = 0;

    // Callbacks
    ButtonOnEventCallback _callback = nullptr;
    ButtonOnEventRepeatCallback _callback_repeating = nullptr;
};

#endif // BUTTONEVENTCALLBACK_H

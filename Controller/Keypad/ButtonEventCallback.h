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
    EventType _type;
    uint16_t _delay;
    uint16_t _max_delay;
    uint16_t _repeat_period;
    uint8_t _execution_count;

    void calculateNextExecutionTime();

    // Callbacks
    ButtonOnEventCallback _callback;
    ButtonOnEventRepeatCallback _callback_repeating;

    // Keep track of when the callback should be executed
    uint16_t _next_execution_time;
};

#endif // BUTTONEVENTCALLBACK_H

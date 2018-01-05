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
#include "PushButton.h"

class ButtonEventCallback {
public:
    ButtonEventCallback();

    PushButtonEventsResponse::EventType getType() const;
    void setType(PushButtonEventsResponse::EventType type);
    void setDelay(uint16_t delay);
    void setMaxDelay(uint16_t max_delay);
    void setRepetitionPeriod(uint16_t repeat_period);
    void setCallback(ButtonOnEventCallback callback);
    void setRepeatingCallback(ButtonOnEventRepeatCallback callback_repeating);

    void executeCallbackIfTime(uint16_t elapsedTime, bool release_event, PushButton& btn);
    void reset();

private:
    PushButtonEventsResponse::EventType _type = PushButtonEventsResponse::EventType::evtUninitialised;
    uint16_t _delay = 0;
    uint16_t _max_delay = 0;
    uint16_t _repeat_period = 0;
    uint8_t _execution_count = 1;
    uint16_t _next_execution_time = 0;

    // Callbacks
    ButtonOnEventCallback _callback = nullptr;
    ButtonOnEventRepeatCallback _callback_repeating = nullptr;

    void calculateNextExecutionTime();
};

#endif // BUTTONEVENTCALLBACK_H

/*
 * ButtonEventCallback.cpp
 * Created: 18/11/2014 19:33:09
 *  Author: Richard
 */ 
 
#include "PushButton.h"
#include "ButtonEventCallback.h"

// Empty default constructor
ButtonEventCallback::ButtonEventCallback()
{}

EventType ButtonEventCallback::getType() const
{
    return this->_type;
}

void ButtonEventCallback::setType(EventType type)
{
    this->_type = type;
}

void ButtonEventCallback::setDelay(uint16_t delay)
{
    this->_delay = delay;
}

void ButtonEventCallback::setMaxDelay(uint16_t max_delay)
{
    this->_max_delay = max_delay;
}

void ButtonEventCallback::setRepetitionPeriod(uint16_t repeat_period)
{
    this->_repeat_period = repeat_period;
}

void ButtonEventCallback::setCallback(ButtonOnEventCallback callback)
{
    this->_callback = callback;
}

void ButtonEventCallback::setRepeatingCallback(ButtonOnEventRepeatCallback callback_repeating)
{
    this->_callback_repeating = callback_repeating;
}

void ButtonEventCallback::executeCallbackIfTime(uint16_t elapsedTime, bool release_event, PushButton& btn)
{
    // Only process callbacks that have been initialised
    if(_type != EventType::evtUninitialised) {
        if (release_event && (_type == EventType::evtRelease)) { // Only check release callbacks at the right time.
            if((elapsedTime > _next_execution_time) && (elapsedTime < _max_delay) && (_execution_count == 1)){
                if(_callback) {
                    _callback(btn, elapsedTime);
                }
            _execution_count++;
            }
        } else if (_type == EventType::evtHold) {
            if((elapsedTime > _next_execution_time) && (_execution_count == 1)) {
                if(_callback) {
                    _callback(btn, elapsedTime);
                }
                _execution_count++;
            }
        } else if (_type == EventType::evtHoldRepeat) {
            if(elapsedTime > _next_execution_time) {
                if(_callback_repeating) {
                    _callback_repeating(btn, elapsedTime, _execution_count);
                }
                calculateNextExecutionTime();
                _execution_count++;
            }
        }
    }
}

void ButtonEventCallback::calculateNextExecutionTime()
{
    this->_next_execution_time += _repeat_period;
}

void ButtonEventCallback::reset()
{
    this->_execution_count = 1;
    this->_next_execution_time = _delay;
}

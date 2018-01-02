/*
 * Button.h
 *
 * Created: 18/11/2014 19:33:02
 *  Author: Richard
 */ 


#ifndef KEYPAD_BUTTONEVENTCALLBACK_H
#define KEYPAD_BUTTONEVENTCALLBACK_H

#include <inttypes.h>
#include <Arduino.h>
#include "Button.h"

class ButtonEventCallback {
public:
    ButtonEventCallback();
    
    EventType getType() const;
    
    void setType(const EventType&);
    void setDelay(uint16_t);
    void setMaxDelay(uint16_t);
    void setRepetitionPeriod(uint16_t);
    void setCallback(ButtonOnEventCallback);
    void setRepeatingCallback(ButtonOnEventRepeatCallback);
    
    void executeCallbackIfTime(uint16_t, bool, Button&);
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





/* ************************************************ *
 * *********** IMPLEMENTATION DETAILS ************* *
 * ************************************************ */
// Empty default constructor
ButtonEventCallback::ButtonEventCallback()  // @suppress("Class members should be properly initialized")
{
	// Initialise variables
	_type = EventType::evtUninitialised;
	_delay = 0;
	_max_delay = 0;
	_repeat_period = 0;
	_execution_count = 1;
	_next_execution_time = 0;
}

EventType ButtonEventCallback::getType() const
{
	return _type;
}

void ButtonEventCallback::setType(const EventType& type)
{
	_type = type;
}

void ButtonEventCallback::setDelay(uint16_t delay)
{
	_delay = delay;
}

void ButtonEventCallback::setMaxDelay(uint16_t max_delay)
{
	_max_delay = max_delay;
}

void ButtonEventCallback::setRepetitionPeriod(uint16_t repeat_period)
{
	_repeat_period = repeat_period;
}

void ButtonEventCallback::setCallback(ButtonOnEventCallback callback)
{
	_callback = callback;
}

void ButtonEventCallback::setRepeatingCallback(ButtonOnEventRepeatCallback callback_repeating)
{
	_callback_repeating = callback_repeating;
}

void ButtonEventCallback::executeCallbackIfTime(uint16_t elapsedTime, bool release_event, Button& btn)
{
	// Only process callbacks that have been initialised
	if(_type != EventType::evtUninitialised){
		if (release_event && _type == EventType::evtRelease){ // Only check release callbacks at the right time.
			if(elapsedTime > _next_execution_time && elapsedTime < _max_delay && _execution_count == 1){
				if(_callback){
					_callback(btn, elapsedTime);
				}
			_execution_count++;
			}
		} else if (_type == EventType::evtHold){
			if(elapsedTime > _next_execution_time && _execution_count == 1){
				if(_callback){
					_callback(btn, elapsedTime);
				}
				_execution_count++;
			}
		} else if (_type == EventType::evtHoldRepeat){
			if(elapsedTime > _next_execution_time){
				if(_callback_repeating){
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

#endif // BUTTONEVENTCALLBACK_H

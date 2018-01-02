/*
 * Button.h
 * Created: 18/11/2014 19:33:02
 * Author: Richard
 *
 * Forked from: https://github.com/r89m/Button
 * Edited: 30/12/2017
 * Editor: Nelson Vides
 */

#ifndef KEYPAD_BUTTON_H_
#define KEYPAD_BUTTON_H_

#include <inttypes.h>
#include <Arduino.h>

// The maximum number of callbacks available to each button. A higher number will use more memory and be (slightly) slower.
constexpr int MAX_CALLBACKS_PER_BUTTON  = 3;

// Forward reference
class Button;

// Define callback types
typedef void (*ButtonOnPressCallback)(Button&);
typedef void (*ButtonOnEventCallback)(Button&, uint16_t);
typedef void (*ButtonOnEventRepeatCallback)(Button&, uint16_t, uint16_t);

enum class EventType {evtUninitialised, evtRelease, evtHold, evtHoldRepeat};
enum class CallbackAttachedResponse {attSuccessful, attNoMoreRoom};

#include "ButtonEventCallback.h"

class Button{
public:
    Button();
    virtual ~Button() = default;
    void onPress(ButtonOnPressCallback);
    CallbackAttachedResponse onRelease(ButtonOnEventCallback) const;
    CallbackAttachedResponse onRelease(uint16_t, ButtonOnEventCallback) const;
    CallbackAttachedResponse onRelease(uint16_t, uint16_t, ButtonOnEventCallback) const;
    CallbackAttachedResponse onHold(uint16_t, ButtonOnEventCallback) const;
    CallbackAttachedResponse onHoldRepeat(uint16_t, uint16_t, ButtonOnEventRepeatCallback) const;
    
    bool update();
    bool is(Button&) const;
    bool isPressed() const;
    
    virtual uint8_t getButtonNumber() const = 0;
protected:
    virtual bool _update_button_state() = 0;

private:
    // data fields
    bool _is_pressed;											    // Whether or not the button is currently pressed
    uint32_t _button_pressed_timestamp;                             // When the button was originally pressed
    ButtonOnPressCallback _on_press_callback;						// A callback for when the button is initially pressed
    ButtonEventCallback _eventCallbacks[MAX_CALLBACKS_PER_BUTTON];	// An array of callbacks for Release, Hold and HoldRepeat events
    
    //methods
    void _button_pressed();
    void _button_released();
    void _button_held();
    void _execute_callbacks(bool);
    uint16_t _button_time_elapsed() const;
    ButtonEventCallback* getNextAvailableCallback() const;
};
  




/* ************************************************ *
 * *********** IMPLEMENTATION DETAILS ************* *
 * ************************************************ */
Button::Button() {// @suppress("Class members should be properly initialized")
	// Initialise variables
	_button_pressed_timestamp = 0;
	_is_pressed = false;
}

void Button::_button_pressed() {
	_is_pressed = true;
	_button_pressed_timestamp = millis();
	
	// Fire the onPress callback if one has been specified
	if (_on_press_callback) {
		_on_press_callback(*this);
	}
	// Reset all callbacks
	for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON; i++) {
	    _eventCallbacks[i].reset();
	}
}

void Button::_button_released()
{
	// Set the button pressed state to false
	_is_pressed = false;
	_execute_callbacks(true);
}

void Button::_button_held()
{
	_execute_callbacks(false);
}

void Button::_execute_callbacks(bool release_event)
{
	uint16_t button_time_elapsed = _button_time_elapsed();
	// Iterate over all callbacks
	for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON - 1; i++) {
		_eventCallbacks[i].executeCallbackIfTime(button_time_elapsed, release_event, *this);
	}
}

uint16_t Button::_button_time_elapsed() const {
	return millis() - _button_pressed_timestamp;
}

bool Button::update()
{
	// Record the previous and new state of the button
	bool _previous_button_state = isPressed();
	bool _new_button_state = _update_button_state();
	
	// Record the new state of the button
	_is_pressed = _new_button_state;
	
	//Serial.println("Button Update");

	// If the state of the button has changed
	if(_previous_button_state != _new_button_state)
	{
		// If the button is now pressed
	    if (_new_button_state == 0) {
	        Serial.print("_new_button_state is "); Serial.println(_new_button_state);
	    }
		if(_new_button_state){ //FIXME this is never called :( ¿still modern?
		    //Serial.println("Execute onPressed callbacks");
			_button_pressed();
		} else {
			// Otherwise if it has just been let go
	        //Serial.println("Execute onReleased callbacks");
			_button_released();
		}
		return true;	// State has changed
		// If the state hasn't changed but the button is pressed - ie it is being held
	}
	else if(_new_button_state)
	{
		_button_held();
		return false;	// State hasn't changed
	}
	else
	{
	    return false;
	}
}

void Button::onPress(ButtonOnPressCallback callback)
{
	_on_press_callback = callback;
}

CallbackAttachedResponse Button::onRelease(ButtonOnEventCallback callback) const
{
	return onRelease(0, -1, callback);
}

CallbackAttachedResponse Button::onRelease(uint16_t wait, ButtonOnEventCallback callback) const
{
	return onRelease(wait, -1, callback);
}

CallbackAttachedResponse Button::onRelease(uint16_t wait, uint16_t max_wait, ButtonOnEventCallback callback) const
{
    ButtonEventCallback* nextCallback = getNextAvailableCallback();

    if (nextCallback != nullptr)
    {
        nextCallback->setType(EventType::evtRelease);
        nextCallback->setDelay(wait);
        nextCallback->setMaxDelay(max_wait);
        nextCallback->setCallback(callback);

        // Now that we're done, let the user know
        return CallbackAttachedResponse::attSuccessful;
    }

    // If we get this far, there was no more space to add a handler
    return CallbackAttachedResponse::attNoMoreRoom;
}

CallbackAttachedResponse Button::onHold(uint16_t duration, ButtonOnEventCallback callback) const
{
    ButtonEventCallback* nextCallback = getNextAvailableCallback();

    if (nextCallback != nullptr)
    {
        nextCallback->setType(EventType::evtHold);
        nextCallback->setDelay(duration);
        nextCallback->setCallback(callback);

        // Now that we're done, let the user know
        return CallbackAttachedResponse::attSuccessful;
    }

    // If we get this far, there was no more space to add a handler
    return CallbackAttachedResponse::attNoMoreRoom;
}

CallbackAttachedResponse Button::onHoldRepeat(uint16_t duration, uint16_t repeat_every, ButtonOnEventRepeatCallback callback) const
{
	ButtonEventCallback* nextCallback = getNextAvailableCallback();
	
	if (nextCallback != nullptr)
	{
		nextCallback->setType(EventType::evtHoldRepeat);
		nextCallback->setDelay(duration);
		nextCallback->setRepetitionPeriod(repeat_every);
		nextCallback->setRepeatingCallback(callback);
		
		// Now that we're done, let the user know
		return CallbackAttachedResponse::attSuccessful;
	}
	
	// If we get this far, there was no more space to add a handler
	return CallbackAttachedResponse::attNoMoreRoom;
}

ButtonEventCallback* Button::getNextAvailableCallback() const
{
	for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON - 1; ++i)
	{
	    // If this callback handler has not be initialised, we can use it
	    if(_eventCallbacks[i].getType() == EventType::evtUninitialised)
		{
		    return &_eventCallbacks[i];
		}
	}
	return nullptr;
}

bool Button::is(Button &btn) const
{
	return (this == &btn);
}

bool Button::isPressed() const
{
	return _is_pressed;
}

#endif /* BUTTON_H_ */

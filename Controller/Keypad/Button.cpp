/*
 * Button.cpp
 * Created: 18/11/2014 19:33:09
 *  Author: Richard
 */ 

#include "Button.h"
#include "ButtonEventCallback.h"

Button::Button()
{}

void Button::_button_pressed()
{
    // Set the button pressed state to true
    this->_is_pressed = true;

    // Record when the button was originally pressed
    this->_button_pressed_timestamp = millis();

    // Fire the onPress callback if one has been specified
    if(this->_on_press_callback){
        this->_on_press_callback(*this);
    }

    // Reset all callbacks
    for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON; i++){
        this->_eventCallbacks[i].reset();
    }
}

void Button::_button_released()
{
    // Set the button pressed state to false
    this->_is_pressed = false;
    this->_execute_callbacks(true);
}

void Button::_button_held()
{
    this->_execute_callbacks(false);
}

void Button::_execute_callbacks(bool release_event)
{
    uint16_t button_time_elapsed = this->_button_time_elapsed();
    // Iterate over all callbacks
    for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON - 1; i++){
        this->_eventCallbacks[i].executeCallbackIfTime(button_time_elapsed, release_event, *this);
    }
}

uint16_t Button::_button_time_elapsed() const
{
    return (millis() - this->_button_pressed_timestamp);
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
    if(_previous_button_state != _new_button_state) {
        // If the button is now pressed
        if(_new_button_state) {
            _button_pressed();
        } else {
            // Otherwise if it has just been let go
            _button_released();
        }
        return true;	// State has changed
    // If the state hasn't changed but the button is pressed - ie it is being held
    } else if(_new_button_state){
        _button_held();
    }
    // If we reach this far, state hasn't changed
    return false;
}

void Button::onPress(ButtonOnPressCallback callback)
{
    this->_on_press_callback = callback;
}

CallbackAttachedResponse Button::onRelease(ButtonOnEventCallback callback)
{
    return onRelease(0, callback);
}

CallbackAttachedResponse Button::onRelease(uint16_t wait, ButtonOnEventCallback callback)
{
    return onRelease(wait, -1, callback);
}

CallbackAttachedResponse Button::onRelease(uint16_t wait, uint16_t max_wait, ButtonOnEventCallback callback)
{
    ButtonEventCallback* nextCallback = this->getNextAvailableCallback();
    if(nextCallback) {
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

CallbackAttachedResponse Button::onHold(uint16_t duration, ButtonOnEventCallback callback)
{
    ButtonEventCallback* nextCallback = getNextAvailableCallback();
    if(nextCallback) {
        nextCallback->setType(EventType::evtHold);
        nextCallback->setDelay(duration);
        nextCallback->setCallback(callback);
        // Now that we're done, let the user know
        return CallbackAttachedResponse::attSuccessful;
    }
    // If we get this far, there was no more space to add a handler
    return CallbackAttachedResponse::attNoMoreRoom;
}

CallbackAttachedResponse Button::onHoldRepeat(uint16_t duration, uint16_t repeat_every, ButtonOnEventRepeatCallback callback)
{
    ButtonEventCallback* nextCallback = getNextAvailableCallback();
    if(nextCallback) {
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

ButtonEventCallback* Button::getNextAvailableCallback()
{
    for(uint8_t i = 0; i < MAX_CALLBACKS_PER_BUTTON - 1; i++){
        // If this callback handler has not be initialised, we can use it
        if(this->_eventCallbacks[i].getType() == EventType::evtUninitialised){
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
    return this->_is_pressed;
}

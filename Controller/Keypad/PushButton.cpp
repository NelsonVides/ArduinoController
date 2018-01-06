/*
 * PushButton.cpp
 * Created: 18/11/2014 19:33:23
 *  Author: Richard
 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Forked by Nelson Vides to implement ANALOG_PINS
 * and modern C++ features
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "PushButton.h"
#include "ButtonEventCallback.h"
using namespace buttonsMgmt;

PushButton::PushButton(uint8_t pin)
        : bouncer(Bouncer(pin))
{
}

bool PushButton::_update_button_state()
{
    // Update the button
    bouncer.update();
    // Return whether it is pressed or not
    return bouncer.read();
}

buttonNumber PushButton::getLastPressedButton()
{
    uint16_t internalValue = this->bouncer.getValue();
    if (internalValue < BouncerConstants::NOISE_TOLERANCE) {
        return this->_last_button_pressed;
    } else {
        if (internalValue > 100 && internalValue < 300) {
            this->_last_button_pressed = buttonNumber::R1;
            return buttonNumber::R1;
        }
        if (internalValue > 300 && internalValue < 550) {
            this->_last_button_pressed = buttonNumber::R2;
            return buttonNumber::R2;
        }
        if (internalValue > 550 && internalValue < 800) {
            this->_last_button_pressed = buttonNumber::R3;
            return buttonNumber::R3;
        }
        if (internalValue > 960) {
            this->_last_button_pressed = buttonNumber::R4;
            return buttonNumber::R4;
        }
        return buttonNumber::btnUnkown;
    }
}

void PushButton::_button_pressed()
{
    // Set the button pressed state to true
    this->_is_pressed = true;

    // Record when the button was originally pressed
    this->_button_pressed_timestamp = millis();

    // Fire the onPress callback if one has been specified
    if (this->_on_press_callback) {
        this->_on_press_callback(*this);
    }

    // Reset all callbacks
    for (uint8_t i = 0; i < PushButtonEventsResponse::MAX_CALLBACKS_PER_BUTTON;
            i++) {
        this->_eventCallbacks[i].reset();
    }
}

void PushButton::_button_released()
{
    // Set the button pressed state to false
    this->_is_pressed = false;
    this->_execute_callbacks(true);
}

void PushButton::_button_held()
{
    this->_execute_callbacks(false);
}

void PushButton::_execute_callbacks(bool release_event)
{
    uint16_t button_time_elapsed = this->_button_time_elapsed();
    // Iterate over all callbacks
    for (uint8_t i = 0;
            i < PushButtonEventsResponse::MAX_CALLBACKS_PER_BUTTON - 1; i++) {
        this->_eventCallbacks[i].executeCallbackIfTime(button_time_elapsed,
                release_event, *this);
    }
}

uint16_t PushButton::_button_time_elapsed() const
{
    return (millis() - this->_button_pressed_timestamp);
}

bool PushButton::update()
{
    // Record the previous and new state of the button
    bool _previous_button_state = isPressed();
    bool _new_button_state = _update_button_state();

    // Record the new state of the button
    _is_pressed = _new_button_state;

    //Serial.println("Button Update");

    // If the state of the button has changed
    if (_previous_button_state != _new_button_state) {
        // If the button is now pressed
        if (_new_button_state) {
            _button_pressed();
        } else {
            // Otherwise if it has just been let go
            _button_released();
        }
        return true;    // State has changed
        // If the state hasn't changed but the button is pressed - ie it is being held
    } else if (_new_button_state) {
        _button_held();
    }
    // If we reach this far, state hasn't changed
    return false;
}

void PushButton::onPress(ButtonOnPressCallback callback)
{
    this->_on_press_callback = callback;
}

PushButtonEventsResponse::CallbackAttachedResponse PushButton::onRelease(
        ButtonOnEventCallback callback)
{
    return onRelease(0, callback);
}

PushButtonEventsResponse::CallbackAttachedResponse PushButton::onRelease(
        uint16_t wait, ButtonOnEventCallback callback)
{
    return onRelease(wait, -1, callback);
}

PushButtonEventsResponse::CallbackAttachedResponse PushButton::onRelease(
        uint16_t wait, uint16_t max_wait, ButtonOnEventCallback callback)
{
    ButtonEventCallback* nextCallback = this->getNextAvailableCallback();
    if (nextCallback) {
        nextCallback->setType(PushButtonEventsResponse::EventType::evtRelease);
        nextCallback->setDelay(wait);
        nextCallback->setMaxDelay(max_wait);
        nextCallback->setCallback(callback);
        // Now that we're done, let the user know
        return PushButtonEventsResponse::CallbackAttachedResponse::attSuccessful;
    }
    // If we get this far, there was no more space to add a handler
    return PushButtonEventsResponse::CallbackAttachedResponse::attNoMoreRoom;
}

PushButtonEventsResponse::CallbackAttachedResponse PushButton::onHold(
        uint16_t duration, ButtonOnEventCallback callback)
{
    ButtonEventCallback* nextCallback = getNextAvailableCallback();
    if (nextCallback) {
        nextCallback->setType(PushButtonEventsResponse::EventType::evtHold);
        nextCallback->setDelay(duration);
        nextCallback->setCallback(callback);
        // Now that we're done, let the user know
        return PushButtonEventsResponse::CallbackAttachedResponse::attSuccessful;
    }
    // If we get this far, there was no more space to add a handler
    return PushButtonEventsResponse::CallbackAttachedResponse::attNoMoreRoom;
}

PushButtonEventsResponse::CallbackAttachedResponse PushButton::onHoldRepeat(
        uint16_t duration, uint16_t repeat_every,
        ButtonOnEventRepeatCallback callback)
{
    ButtonEventCallback* nextCallback = getNextAvailableCallback();
    if (nextCallback) {
        nextCallback->setType(
                PushButtonEventsResponse::EventType::evtHoldRepeat);
        nextCallback->setDelay(duration);
        nextCallback->setRepetitionPeriod(repeat_every);
        nextCallback->setRepeatingCallback(callback);
        // Now that we're done, let the user know
        return PushButtonEventsResponse::CallbackAttachedResponse::attSuccessful;
    }
    // If we get this far, there was no more space to add a handler
    return PushButtonEventsResponse::CallbackAttachedResponse::attNoMoreRoom;
}

ButtonEventCallback* PushButton::getNextAvailableCallback()
{
    for (uint8_t i = 0;
            i < PushButtonEventsResponse::MAX_CALLBACKS_PER_BUTTON - 1; i++) {
        // If this callback handler has not be initialised, we can use it
        if (this->_eventCallbacks[i].getType()
                == PushButtonEventsResponse::EventType::evtUninitialised) {
            return &_eventCallbacks[i];
        }
    }
    return nullptr;
}

bool PushButton::is(PushButton &btn) const
{
    return (this == &btn);
}

bool PushButton::isPressed() const
{
    return this->_is_pressed;
}

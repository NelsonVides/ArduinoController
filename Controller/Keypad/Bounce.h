/*
  The MIT License (MIT)
  Copyright (c) 2013 thomasfredericks
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * Bounce.h
 *  Created on: Dec 30, 2017
 *  Forked from: https://github.com/thomasfredericks/Bounce2
 *  	Set to control analog buttons as well, considering the different input values.
 *  	Got rid of portability boilerplate.
 *  Author: Nelson Vides
 */

#ifndef KEYPAD_BOUNCE_H
#define KEYPAD_BOUNCE_H

// Uncomment the following line for "LOCK-OUT" debounce method
//#define BOUNCE_LOCK_OUT
// Uncomment the following line for "BOUNCE_WITH_PROMPT_DETECTION" debounce method
//#define BOUNCE_WITH_PROMPT_DETECTION

#include <Arduino.h>
#include <inttypes.h>

constexpr uint8_t bitLeftShiftOfOne(const uint8_t n){
    return (1 << n);
}

class Bounce {
public:
    // Create an instance of the bounce library
    explicit Bounce(const uint8_t pin);

    // Sets the debounce interval
    void setInterval(const unsigned long interval_millis);

    // Updates the pin
    // Returns true if changed, false if not
    bool update();

    // Returns the updated pin state
    bool read() const;

    // Returns the falling pin state
    bool fell() const;

    // Returns the rising pin state
    bool rose() const;

    // Returns the updated pin state
    uint16_t getValue() const;

protected:
    uint8_t _state;
    uint16_t _value;
    const uint8_t _pin;
    unsigned long _interval_millis;
    unsigned long _previous_millis;
};




/* ************************************************ *
 * *********** IMPLEMENTATION DETAILS ************* *
 * ************************************************ */
constexpr uint8_t DEBOUNCED_STATE = 0;
constexpr uint8_t UNSTABLE_STATE = 1;
constexpr uint8_t STATE_CHANGED = 3;

Bounce::Bounce(const uint8_t pin)
    : _value(0), _state(0), _interval_millis(10), _previous_millis(0), _pin(pin)
{
    this->_state = 0;
    if (analogRead(pin) > 0)
    {
        this->_state = bitLeftShiftOfOne(DEBOUNCED_STATE) | bitLeftShiftOfOne(UNSTABLE_STATE);
    }
    #ifdef BOUNCE_LOCK_OUT
        this->_previous_millis = 0;
    #else
        this->_previous_millis = micros();
    #endif
}

void Bounce::setInterval(const unsigned long interval_millis)
{
    this->_interval_millis = interval_millis;
}

bool Bounce::update()
{
#ifdef BOUNCE_LOCK_OUT
    this->_state &= ~bitLeftShiftOfOne(STATE_CHANGED);
    // Ignore everything if we are locked out
    if (micros() - this->_previous_millis >= this->_interval_millis)
    {
        this->_value = analogRead(this->_pin);
        bool currentState = this->_value;
        if (currentState != (bool)(this->_state & bitLeftShiftOfOne(DEBOUNCED_STATE)))
        {
            this->_previous_millis = micros();
            this->_state ^= bitLeftShiftOfOne(DEBOUNCED_STATE);
            this->_state |= bitLeftShiftOfOne(STATE_CHANGED);
        }
    }
    return this->_state & bitLeftShiftOfOne(STATE_CHANGED);

#elif defined BOUNCE_WITH_PROMPT_DETECTION
    // Read the state of the switch port into a temporary variable.
    this->_value = analogRead(this->_pin);
    bool readState = this->_value;

    // Clear Changed State Flag - will be reset if we confirm a button state change.
    this->_state &= ~bitLeftShiftOfOne(STATE_CHANGED);

    if (readState != (bool)(this->_state & bitLeftShiftOfOne(DEBOUNCED_STATE)))
    {
		// We have seen a change from the current button state.
		if (micros() - this->_previous_millis >= this->_interval_millis)
		{
			// We have passed the time threshold, so a new change of state is allowed.
			// set the STATE_CHANGED flag and the new DEBOUNCED_STATE.
			// This will be prompt as long as there has been greater than interval_misllis ms since last change of input.
			// Otherwise debounced state will not change again until bouncing is stable for the timeout period.
			this->_state ^= bitLeftShiftOfOne(DEBOUNCED_STATE);
			this->_state |= bitLeftShiftOfOne(STATE_CHANGED);
		}
    }

    // If the readState is different from previous readState, reset the debounce timer - as input is still unstable
    // and we want to prevent new button state changes until the previous one has remained stable for the timeout.
    if (readState != (bool)(this->_state & bitLeftShiftOfOne(UNSTABLE_STATE)))
    {
    	// Update Unstable Bit to macth readState
        this->_state ^= bitLeftShiftOfOne(UNSTABLE_STATE);
        this->_previous_millis = micros();
    }
    // return just the sate changed bit
    return this->_state & bitLeftShiftOfOne(STATE_CHANGED);
#else
    //Serial.println("Update bouncer loop");
    // Read the state of the switch in a temporary variable.
    this->_value = analogRead(this->_pin);
    bool currentState = (this->_value > 50);
    this->_state &= ~bitLeftShiftOfOne(STATE_CHANGED); ///reset the STATE_CHANGED flag

    // If the reading is different from last reading, reset the debounce counter
    if (currentState != (bool)(_state & bitLeftShiftOfOne(UNSTABLE_STATE))) ///PRESSED_STATE != UNSTABLE_STATE
    {
        _previous_millis = micros();
        _state ^= bitLeftShiftOfOne(UNSTABLE_STATE); ///XOR-on the UNSTABLE_STATE flag, reset all other flags
    }
    if ((micros() - this->_previous_millis) >= this->_interval_millis)
    {
        // We have passed the threshold time, so the input is now stable
        // If it is different from last state, set the STATE_CHANGED flag
        if (currentState != (bool)(this->_state & bitLeftShiftOfOne(DEBOUNCED_STATE))) ///PRESSED_STATE != DEBOUNCED_STATE
        {
            _previous_millis = micros();
            _state ^= bitLeftShiftOfOne(DEBOUNCED_STATE); ///XOR 0001
            _state |= bitLeftShiftOfOne(STATE_CHANGED);   ///iOR 1000
            Serial.print("EUREKA! "); Serial.println(_state);
        }
    }
    return _state & bitLeftShiftOfOne(STATE_CHANGED); ///AND 1000
#endif
}

bool Bounce::read() const { // false if xxx0; returns DEBOUNCED_STATE flag
    return (this->_state & bitLeftShiftOfOne(DEBOUNCED_STATE));
}

bool Bounce::rose() const { // false if x0x0; returns if DEBOUNCED and CHANGED
    return (this->_state & bitLeftShiftOfOne(DEBOUNCED_STATE)) && (this->_state & bitLeftShiftOfOne(STATE_CHANGED));
}

bool Bounce::fell() const { // false if x1x1; returns if NOT DEBOUNCED or NOT CHANGED
    return !rose();
}

uint16_t Bounce::getValue() const {
    return this->_value;
}

#endif

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
 *  	Set to control analog buttons as well.
 *  	Got rid of portability boilerplate.
 *  Author: Nelson Vides
 */

#ifndef CLASSES_BOUNCE_H_
#define CLASSES_BOUNCE_H_

// Uncomment the following line for "LOCK-OUT" debounce method
//#define BOUNCE_LOCK_OUT
// Uncomment the following line for "BOUNCE_WITH_PROMPT_DETECTION" debounce method
//#define BOUNCE_WITH_PROMPT_DETECTION

#include <Arduino.h>
#include <inttypes.h>

constexpr int n_BV(const int n)
{
    return (1 << n);
}

class Bounce {
public:
    // Create an instance of the bounce library
    Bounce(const int pin);

    // Attach to a pin (and also sets initial state)
    void setPin(const int pin);

    // Sets the debounce interval
    void setInterval(uint16_t interval_millis);

    // Updates the pin
    // Returns 1 if the state changed
    // Returns 0 if the state did not change
    const bool update();

    // Returns the updated pin state
    const bool read() const;

    // Returns the falling pin state
    const bool fell() const;

    // Returns the rising pin state
    const bool rose() const;

protected:
    unsigned long _previous_millis;
    uint16_t _interval_millis;
    uint8_t _state;
    const uint8_t _pin;
};


constexpr int DEBOUNCED_STATE = 0;
constexpr int UNSTABLE_STATE = 1;
constexpr int STATE_CHANGED = 3;

Bounce::Bounce(const int pin)
    : _previous_millis(0)
    , _interval_millis(10)
    , _state(0)
    , _pin(_pin)
{
    this->setPin(pin);
}

void Bounce::setPin(const int pin)
{
    _state = 0;
    if (analogRead(pin)) {
        _state = n_BV(DEBOUNCED_STATE) | n_BV(UNSTABLE_STATE);
    }
#ifdef BOUNCE_LOCK_OUT
    this->_previous_millis = 0;
#else
    this->_previous_millis = micros();
#endif
}

void Bounce::setInterval(const uint16_t interval_millis)
{
    this->_interval_millis = interval_millis;
}

const bool Bounce::update()
{
#ifdef BOUNCE_LOCK_OUT
    this->_state &= ~n_BV(STATE_CHANGED);
    // Ignore everything if we are locked out
    if (micros() - this->_previous_millis >= this->_interval_millis)
    {
        bool currentState = digitalRead(this->_pin);
        if ((bool)(this->_state & n_BV(DEBOUNCED_STATE)) != currentState)
        {
            this->_previous_millis = micros();
            this->_state ^= n_BV(DEBOUNCED_STATE);
            this->_state |= n_BV(STATE_CHANGED);
        }
    }
    return this->_state & n_BV(STATE_CHANGED);

#elif defined BOUNCE_WITH_PROMPT_DETECTION
    // Read the state of the switch port into a temporary variable.
    bool readState = digitalRead(pin);

    // Clear Changed State Flag - will be reset if we confirm a button state change.
    this->_state &= ~n_BV(STATE_CHANGED);

    if ( readState != (bool)(this->_state & n_BV(DEBOUNCED_STATE)))
    {
		// We have seen a change from the current button state.
		if ( micros() - this->_previous_millis >= this->_interval_millis )
		{
			// We have passed the time threshold, so a new change of state is allowed.
			// set the STATE_CHANGED flag and the new DEBOUNCED_STATE.
			// This will be prompt as long as there has been greater than interval_misllis ms since last change of input.
			// Otherwise debounced state will not change again until bouncing is stable for the timeout period.
			this->_state ^= n_BV(DEBOUNCED_STATE);
			this->_state |= n_BV(STATE_CHANGED);
		}
    }

    // If the readState is different from previous readState, reset the debounce timer - as input is still unstable
    // and we want to prevent new button state changes until the previous one has remained stable for the timeout.
    if ( readState != (bool)(this->_state & n_BV(UNSTABLE_STATE)) )
    {
    	// Update Unstable Bit to macth readState
        this->_state ^= n_BV(UNSTABLE_STATE);
        this->_previous_millis = micros();
    }
    // return just the sate changed bit
    return this->_state & n_BV(STATE_CHANGED);
#else
    // Read the state of the switch in a temporary variable.
    bool currentState = analogRead(this->_pin);
    this->_state &= ~n_BV(STATE_CHANGED);

    // If the reading is different from last reading, reset the debounce counter
    if ( currentState != (bool)(this->_state & n_BV(UNSTABLE_STATE)) ) {
        this->_previous_millis = micros();
        this->_state ^= n_BV(UNSTABLE_STATE);
    } else if ( micros() - this->_previous_millis >= this->_interval_millis ) {
        // We have passed the threshold time, so the input is now stable
        // If it is different from last state, set the STATE_CHANGED flag
        if ((bool)(this->_state & n_BV(DEBOUNCED_STATE)) != currentState) {
            this->_previous_millis = micros();
            this->_state ^= n_BV(DEBOUNCED_STATE);
            this->_state |= n_BV(STATE_CHANGED);
        }
    }
    return this->_state & n_BV(STATE_CHANGED);
#endif
}

const bool Bounce::read() const
{
    return this->_state & n_BV(DEBOUNCED_STATE);
}

const bool Bounce::rose() const
{
    return ( this->_state & n_BV(DEBOUNCED_STATE) ) && ( this->_state & n_BV(STATE_CHANGED));
}

const bool Bounce::fell() const
{
    return !( this->_state & n_BV(DEBOUNCED_STATE) ) && ( this->_state & n_BV(STATE_CHANGED));
}

#endif

// Please read Bounce2.h for information about the liscence and authors

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Bounce2.h"

#define DEBOUNCED_STATE 0
#define UNSTABLE_STATE  1
#define STATE_CHANGED   3


Bounce::Bounce()
    : previous_millis(0)
    , interval_millis(10)
    , state(0)
    , pin(0)
{}

void Bounce::attach(int pin) {
    this->pin = pin;
    state = 0;
    if (digitalRead(pin)) {
        state = SetBouncerFlag(DEBOUNCED_STATE) | SetBouncerFlag(UNSTABLE_STATE);
    }
#ifdef BOUNCE_LOCK_OUT
    previous_millis = 0;
#else
    previous_millis = millis();
#endif
}

void Bounce::attach(int pin, int mode){
#if defined(ARDUINO_STM_NUCLEO_F103RB) || defined(ARDUINO_GENERIC_STM32F103C)
    pinMode(pin, (WiringPinMode)mode);
#else
    pinMode(pin, mode);
#endif
  this->attach(pin);
}

void Bounce::interval(uint16_t interval_millis)
{
    this->interval_millis = interval_millis;
}

bool Bounce::update()
{
#ifdef BOUNCE_LOCK_OUT
    state &= ~SetBouncerFlag(STATE_CHANGED);
    // Ignore everything if we are locked out
    if (millis() - previous_millis >= interval_millis) {
        bool currentState = digitalRead(pin);
        if ((bool)(state & SetBouncerFlag(DEBOUNCED_STATE)) != currentState) {
            previous_millis = millis();
            state ^= SetBouncerFlag(DEBOUNCED_STATE);
            state |= SetBouncerFlag(STATE_CHANGED);
        }
    }
    return state & SetBouncerFlag(STATE_CHANGED);

#elif defined BOUNCE_WITH_PROMPT_DETECTION
    // Read the state of the switch port into a temporary variable.
    bool readState = digitalRead(pin);

    // Clear Changed State Flag - will be reset if we confirm a button state change.
    state &= ~SetBouncerFlag(STATE_CHANGED);

    if ( readState != (bool)(state & SetBouncerFlag(DEBOUNCED_STATE))) {
      // We have seen a change from the current button state.

      if ( millis() - previous_millis >= interval_millis ) {
	// We have passed the time threshold, so a new change of state is allowed.
	// set the STATE_CHANGED flag and the new DEBOUNCED_STATE.
	// This will be prompt as long as there has been greater than interval_misllis ms since last change of input.
	// Otherwise debounced state will not change again until bouncing is stable for the timeout period.
	state ^= SetBouncerFlag(DEBOUNCED_STATE);
	state |= SetBouncerFlag(STATE_CHANGED);
      }
    }

    // If the readState is different from previous readState, reset the debounce timer - as input is still unstable
    // and we want to prevent new button state changes until the previous one has remained stable for the timeout.
    if ( readState != (bool)(state & SetBouncerFlag(UNSTABLE_STATE)) ) {
	// Update Unstable Bit to macth readState
        state ^= SetBouncerFlag(UNSTABLE_STATE);
        previous_millis = millis();
    }
    // return just the sate changed bit
    return state & SetBouncerFlag(STATE_CHANGED);
#else
    // Read the state of the switch in a temporary variable.
    bool currentState = digitalRead(pin);
    state &= ~SetBouncerFlag(STATE_CHANGED);

    // If the reading is different from last reading, reset the debounce counter
    if ( currentState != (bool)(state & SetBouncerFlag(UNSTABLE_STATE)) ) {
        previous_millis = millis();
        state ^= SetBouncerFlag(UNSTABLE_STATE);
    } else
        if ( millis() - previous_millis >= interval_millis ) {
            // We have passed the threshold time, so the input is now stable
            // If it is different from last state, set the STATE_CHANGED flag
            if ((bool)(state & SetBouncerFlag(DEBOUNCED_STATE)) != currentState) {
                previous_millis = millis();
                state ^= SetBouncerFlag(DEBOUNCED_STATE);
                state |= SetBouncerFlag(STATE_CHANGED);
            }
        }

    return state & SetBouncerFlag(STATE_CHANGED);
#endif
}

bool Bounce::read()
{
    return state & SetBouncerFlag(DEBOUNCED_STATE);
}

bool Bounce::rose()
{
    return ( state & SetBouncerFlag(DEBOUNCED_STATE) ) && ( state & SetBouncerFlag(STATE_CHANGED));
}

bool Bounce::fell()
{
    return !( state & SetBouncerFlag(DEBOUNCED_STATE) ) && ( state & SetBouncerFlag(STATE_CHANGED));
}

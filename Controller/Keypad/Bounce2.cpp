// Please read Bounce2.h for information about the liscence and authors

#include "Arduino.h"
#include "Bounce2.h"

Bounce::Bounce()
    : previous_millis(0)
    , interval_millis(10)
    , state(0)
    , value(0)
    , pin(0)
{}

void Bounce::attach(uint8_t pin) {
    this->pin = pin;
    state = 0;
    if (analogRead(pin) > 10) {
        state = SetBouncerFlag(DEBOUNCED_STATE) | SetBouncerFlag(UNSTABLE_STATE);
    }
#ifdef BOUNCE_LOCK_OUT
    previous_millis = 0;
#else
    previous_millis = millis();
#endif
}

void Bounce::attach(uint8_t pin, uint8_t mode){
    pinMode(pin, mode);
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
        this->value = analogRead(this->pin);
        bool currentState = (this->value > 50);
        if ((bool)(state & SetBouncerFlag(DEBOUNCED_STATE)) != currentState) {
            previous_millis = millis();
            state ^= SetBouncerFlag(DEBOUNCED_STATE);
            state |= SetBouncerFlag(STATE_CHANGED);
        }
    }
    return state & SetBouncerFlag(STATE_CHANGED);

#elif defined BOUNCE_WITH_PROMPT_DETECTION
    // Read the state of the switch port into a temporary variable.
    this->value = analogRead(this->pin);
    bool currentState = (this->value > 50);

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
    this->value = analogRead(this->pin);
    bool currentState = (this->value > 50);
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

bool Bounce::read() const
{
    return state & SetBouncerFlag(DEBOUNCED_STATE);
}

bool Bounce::rose() const
{
    return ( state & SetBouncerFlag(DEBOUNCED_STATE) ) && ( state & SetBouncerFlag(STATE_CHANGED));
}

bool Bounce::fell() const
{
    return !rose();
}

uint16_t Bounce::getValue() const {
    return value;
}

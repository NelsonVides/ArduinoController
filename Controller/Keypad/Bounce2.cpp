// Please read Bounce2.h for information about the license and authors

#include "Arduino.h"
#include "Bounce2.h"

//This is an implementation detail that doesn't need to be seen by the outside world.
namespace {
    constexpr uint8_t SetBouncerFlag(const uint8_t n){
        return (1 << n);
    }
    constexpr uint8_t DEBOUNCED_STATE = 0b00000000;
    constexpr uint8_t UNSTABLE_STATE  = 0b00000001;
    constexpr uint8_t STATE_CHANGED   = 0b00000011;
}

Bounce::Bounce(uint8_t pin)
    : previous_millis(0)
    , interval_millis(10)
    , state(0)
    , value(0)
    , pin(pin)
{
    this->attach(pin);
}

void Bounce::attach(uint8_t pin) {
    if (digitalRead(pin)) {
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
        #ifdef ANALOG_PINS
            this->value = analogRead(this->pin);
            bool currentState = (this->value > 50);
        #else
            bool currentState = digitalRead(this->pin);
        #endif
        if ((bool)(state & SetBouncerFlag(DEBOUNCED_STATE)) != currentState) {
            previous_millis = millis();
            state ^= SetBouncerFlag(DEBOUNCED_STATE);
            state |= SetBouncerFlag(STATE_CHANGED);
        }
    }
    return state & SetBouncerFlag(STATE_CHANGED);

#elif defined BOUNCE_WITH_PROMPT_DETECTION
    // Read the state of the switch port into a temporary variable.
    #ifdef ANALOG_PINS
        this->value = analogRead(this->pin);
        bool currentState = (this->value > 50);
    #else
        bool currentState = digitalRead(this->pin);
    #endif
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
    #ifdef ANALOG_PINS
        this->value = analogRead(this->pin);
        bool currentState = (this->value > 50);
    #else
        bool currentState = digitalRead(this->pin);
    #endif
    state &= ~SetBouncerFlag(STATE_CHANGED); ///reset STATE_CHANGED flag

    // If the reading is different from last reading, reset the debounce counter
    if (currentState != (bool)(state & SetBouncerFlag(UNSTABLE_STATE))) /// PRESSED_STATE != UNSTABLE_STATE
    {
        previous_millis = millis();
        state ^= SetBouncerFlag(UNSTABLE_STATE); ///XOR-on the UNSTABLE_STATE, reset all other flags
    }
    if ( millis() - previous_millis >= interval_millis) ///if enought time has passed
    {
        // We have passed the threshold time, so the input is now stable
        // If it is different from last state, set the STATE_CHANGED flag
        if (currentState != (bool)(state & SetBouncerFlag(DEBOUNCED_STATE))) ///PRESSED_STATE != DEBOUNCED_STATE
        {
            previous_millis = millis();
            state ^= SetBouncerFlag(DEBOUNCED_STATE); /// XOR 0001 =>
            state |= SetBouncerFlag(STATE_CHANGED);   /// iOR 1000 => turn on the STATE_CHANGED flag
        }
    }
    return state & SetBouncerFlag(STATE_CHANGED); // return STATE_CHANGED flag
#endif
}

bool Bounce::read() const ///return if DEBOUNCED
{
    return state & SetBouncerFlag(DEBOUNCED_STATE);
}

bool Bounce::rose() const ///return if DEBOUNCED and CHANGED
{
    return ( state & SetBouncerFlag(DEBOUNCED_STATE) ) && ( state & SetBouncerFlag(STATE_CHANGED));
}

bool Bounce::fell() const ///return if !DEBOUNCED or !CHANGED
{
    return !rose();
}

uint16_t Bounce::getValue() const {
    return this->value;
}

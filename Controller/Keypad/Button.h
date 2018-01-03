/*
 * Button.h
 * Created: 18/11/2014 19:33:02
 *  Author: Richard
 */ 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Forked by Nelson Vides to implement ANALOG_PINS
 * and modern C++ features
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <inttypes.h>
#include <Arduino.h>

// The maximum number of callbacks available to each button. A higher number will use more memory and be (slightly) slower
constexpr uint8_t MAX_CALLBACKS_PER_BUTTON = 3;

// Forward reference
class Button;

// Define callback types
typedef void (*ButtonOnPressCallback)(Button&);
typedef void (*ButtonOnEventCallback)(Button&, uint16_t);
typedef void (*ButtonOnEventRepeatCallback)(Button&, uint16_t, uint16_t);

enum class EventType {evtUninitialised, evtRelease, evtHold, evtHoldRepeat};
enum class CallbackAttachedResponse {attSuccessful, attNoMoreRoom};

#include "ButtonEventCallback.h"

class Button {
public:
    Button();
    virtual ~Button() = default;

    /// ButtonOnPressCallback is a function which will be called as soon as the button is pressed.
    /// It must be defined with the parameters shown below
    /// void callThisFunctionOnPress(Button& btn);
    void onPress(ButtonOnPressCallback);

    /// ButtonOnEventCallback is a function which is called when the button is released.
    /// It must be defined with the parameters shown below
    /// void callThisFunctionOnRelease(Button& btn, uint_16t duration);
    CallbackAttachedResponse onRelease(ButtonOnEventCallback);
    /// as above, plus wait if the button is held for at-least waitms
    /// onReleaseCallbackFunction will be called.
    CallbackAttachedResponse onRelease(uint16_t, ButtonOnEventCallback);
    /// as above, plus max_wait if the button is held for more than max_waitms
    /// onReleaseCallbackFunction will not be called.
    CallbackAttachedResponse onRelease(uint16_t, uint16_t, ButtonOnEventCallback);

    /// duration how long the button must be held before onHoldCallbackFunction is called.
    /// onHoldCallbackFunction is a function which is called when the button is held.
    /// It must be defined with the parameters shown below
    ///void callThisFuntionOnHold(Button& btn, uint_16t duration)
    CallbackAttachedResponse onHold(uint16_t, ButtonOnEventCallback);

    ///duration how long the button must be held before onHoldAndRepeatCallbackFunction is called. repeat_every how long to wait before onHoldAndRepeatCallbackFunction is called repeatedly. onHoldAndRepeatCallbackFunction is a function which is called repeatedly when the button is held. It must be defined with the parameters shown below
    ///void callThisFunctionOnHoldAndRepeat(Button& btn, uint16_t duration, uint8_t repeat_count)
    CallbackAttachedResponse onHoldRepeat(uint16_t, uint16_t, ButtonOnEventRepeatCallback);

    /// Update the button state - this will call any callbacks that are necessary.
    /// Returns true if the state changes.
    bool update();

    /// Return whether or not the button is the same as the btn passed
    bool is(Button&) const;

    /// Return whether or not the button is currently pressed.
    bool isPressed() const;

protected:
    virtual bool _update_button_state() = 0;

private:
    uint32_t _button_pressed_timestamp;								// When the button was originally pressed
    bool _is_pressed;											// Whether or not the button is currently pressed

    ButtonOnPressCallback _on_press_callback;						// A callback for when the button is initially pressed
    ButtonEventCallback _eventCallbacks[MAX_CALLBACKS_PER_BUTTON];	// An array of callbacks for Release, Hold and HoldRepeat events

    void _button_pressed();
    void _button_released();
    void _button_held();
    uint16_t _button_time_elapsed() const;
    void _execute_callbacks(bool);
    ButtonEventCallback* getNextAvailableCallback();
};
  
#endif /* BUTTON_H_ */

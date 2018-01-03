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
#define MAX_CALLBACKS_PER_BUTTON 3

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
    void onPress(ButtonOnPressCallback);
    CallbackAttachedResponse onRelease(ButtonOnEventCallback);
    CallbackAttachedResponse onRelease(uint16_t, ButtonOnEventCallback);
    CallbackAttachedResponse onRelease(uint16_t, uint16_t, ButtonOnEventCallback);
    CallbackAttachedResponse onHold(uint16_t, ButtonOnEventCallback);
    CallbackAttachedResponse onHoldRepeat(uint16_t, uint16_t, ButtonOnEventRepeatCallback);

    bool update();
    bool is(Button&) const;
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

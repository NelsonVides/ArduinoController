/*
 * PushButton.h
 * Created: 18/11/2014 19:33:33
 *  Author: Richard
 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Forked by Nelson Vides to implement ANALOG_PINS
 * and modern C++ features
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

#include "Bouncer.h"    // https://github.com/thomasfredericks/Bounce-Arduino-Wiring

namespace buttonsMgmt {
    // Forward reference
    class PushButton;

    // Define callback types
    using ButtonOnPressCallback = void(*)(PushButton&);
    using ButtonOnEventCallback = void(*)(PushButton&, uint16_t);
    using ButtonOnEventRepeatCallback = void(*)(PushButton&, uint16_t, uint16_t);

    namespace PushButtonEventsResponse {
        // The maximum number of callbacks available to each button. A higher number will use more memory and be (slightly) slower
        constexpr uint8_t MAX_CALLBACKS_PER_BUTTON = 3;
        enum class EventType {
            evtUninitialised, evtRelease, evtHold, evtHoldRepeat
        };
        enum class CallbackAttachedResponse {
            attSuccessful, attNoMoreRoom
        };
    }
}

/* There is a circular dependency on these two libraries, so this should not be removed
 * First, the Forward references and function-pointers are defined.
 * Then, ButtonEventCallback enters definition, and this only uses pointers or references
 * to PushButton, so a forward referece sufices.
 * Then, PushButton enters, which does use both pointers and common objects of type
 * ButtonEventCallback, hence, it needs it to be fully defined already.
 */
#include "ButtonEventCallback.h"

namespace buttonsMgmt {
    class PushButton {
    public:
        explicit PushButton(uint8_t pin);

        /// ButtonOnPressCallback is a function which will be called as soon as the button is pressed.
        /// It must be defined with the parameters shown below
        /// void callThisFunctionOnPress(Button& btn);
        void onPress(ButtonOnPressCallback);

        /// ButtonOnEventCallback is a function which is called when the button is released.
        /// It must be defined with the parameters shown below
        /// void callThisFunctionOnRelease(Button& btn, uint_16t duration);
        PushButtonEventsResponse::CallbackAttachedResponse onRelease(
                ButtonOnEventCallback);
        /// as above, plus wait if the button is held for at-least waitms
        /// onReleaseCallbackFunction will be called.
        PushButtonEventsResponse::CallbackAttachedResponse onRelease(uint16_t,
                ButtonOnEventCallback);
        /// as above, plus max_wait if the button is held for more than max_waitms
        /// onReleaseCallbackFunction will not be called.
        PushButtonEventsResponse::CallbackAttachedResponse onRelease(uint16_t,
                uint16_t, ButtonOnEventCallback);

        /// duration how long the button must be held before onHoldCallbackFunction is called.
        /// onHoldCallbackFunction is a function which is called when the button is held.
        /// It must be defined with the parameters shown below
        ///void callThisFuntionOnHold(Button& btn, uint_16t duration)
        PushButtonEventsResponse::CallbackAttachedResponse onHold(uint16_t,
                ButtonOnEventCallback);

        ///duration how long the button must be held before onHoldAndRepeatCallbackFunction is called. repeat_every how long to wait before onHoldAndRepeatCallbackFunction is called repeatedly. onHoldAndRepeatCallbackFunction is a function which is called repeatedly when the button is held. It must be defined with the parameters shown below
        ///void callThisFunctionOnHoldAndRepeat(Button& btn, uint16_t duration, uint8_t repeat_count)
        PushButtonEventsResponse::CallbackAttachedResponse onHoldRepeat(
                uint16_t, uint16_t, ButtonOnEventRepeatCallback);

        /// Update the button state - this will call any callbacks that are necessary.
        /// Returns true if the state changes.
        bool update();

        /// Return whether or not the button is the same as the btn passed
        bool is(PushButton&) const;

        /// Return whether or not the button is currently pressed.
        bool isPressed() const;

        /// Returns the buttonNumber in this pin.
        int16_t getAnalogValue();

    protected:
        bool _update_button_state();

    private:
        Bouncer bouncer;
        uint32_t _button_pressed_timestamp = 0; // When the button was originally pressed
        bool _is_pressed = false; // Whether or not the button is currently pressed
        ButtonOnPressCallback _on_press_callback = nullptr; // A callback for when the button is initially pressed
        ButtonEventCallback _eventCallbacks[PushButtonEventsResponse::MAX_CALLBACKS_PER_BUTTON]; // An array of callbacks for Release, Hold and HoldRepeat events

        void _button_pressed();
        void _button_released();
        void _button_held();
        uint16_t _button_time_elapsed() const;
        void _execute_callbacks(bool release_event);
        ButtonEventCallback* getNextAvailableCallback();
    };
}

#endif /* PUSHBUTTON_H_ */

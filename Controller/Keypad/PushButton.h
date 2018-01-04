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

#include "Bouncer.h"
#include "Button.h"

constexpr uint8_t PRESSED_WHEN_HIGH      = 0b00000000;
constexpr uint8_t PRESSED_WHEN_LOW       = 0b00000001;
constexpr uint8_t ENABLE_INTERNAL_PULLUP = 0b00000010;

// Define button configuration callback
using PushButtonConfigurationCallback = void(*)(Bouncer&);

class PushButton : public Button {
public:
    PushButton(uint8_t pin);

    virtual ~PushButton() = default;
    uint16_t getAnalogValue() const; //TODO: make this readable for the callbacks

protected:
    bool _update_button_state();

private:
    Bouncer bouncer;
};

#endif /* PUSHBUTTON_H_ */

/*
 * PushButton.cpp
 *
 * Created: 18/11/2014 19:33:23
 *  Author: Richard
 */ 

#include "PushButton.h"

PushButton::PushButton(uint8_t pin)
    : bouncer(Bounce(pin))
{
    init(pin, INPUT);
}

PushButton::PushButton(uint8_t pin, uint8_t options)
    : bouncer(Bounce(pin))
{
    init(pin, options);
}

void PushButton::init(uint8_t pin, uint8_t options)
{
    if (options & ENABLE_INTERNAL_PULLUP){
        bouncer.attach(pin, INPUT_PULLUP);
    } else {
        bouncer.attach(pin, INPUT);
    }
    if ((options & PRESSED_WHEN_LOW) || (options & ENABLE_INTERNAL_PULLUP)){
        _button_pressed_value = LOW;
    }
    else {
        _button_pressed_value = HIGH;
    }
}

void PushButton::configureButton(PushButtonConfigurationCallback configurationCallback)
{
    configurationCallback(bouncer);
}

boolean PushButton::_update_button_state()
{
    // Update the button
    bouncer.update();
    // Return whether it is pressed or not
    return (bouncer.read() == _button_pressed_value);
}

/*
 * PushButton.cpp
 * Created: 18/11/2014 19:33:23
 *  Author: Richard
 */ 

#include "PushButton.h"

PushButton::PushButton(uint8_t pin)
    : bouncer(Bouncer(pin))
{}

boolean PushButton::_update_button_state()
{
    // Update the button
    bouncer.update();
    // Return whether it is pressed or not
    return bouncer.read();
}

uint16_t PushButton::getAnalogValue() const
{
    return this->bouncer.getValue();
}

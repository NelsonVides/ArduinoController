/*
 * Keypad.cpp
 *  Created on: Jan 4, 2018
 *      Author: Nelson Vides
 */

#include "Keypad.h"

Keypad::Keypad(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
   : _rowFourButtons({PushButton(pin1),
                      PushButton(pin2),
                      PushButton(pin3),
                      PushButton(pin4)})
{}

void Keypad::updateKeys() {
    for (PushButton btn : this->_rowFourButtons) {
        btn.update();
    }
}

buttonNumber Keypad::whichButtonIsPressed() const {
    int internalValue;
    if (this->_rowFourButtons[0].isPressed()) {
        internalValue = this->_rowFourButtons[0].getAnalogValue();
        if (internalValue>100 && internalValue<300)
            return buttonNumber::S1;
        if (internalValue>300 && internalValue<550)
            return buttonNumber::S2;
        if (internalValue>550 && internalValue<800)
            return buttonNumber::S3;
        if (internalValue>960)
            return buttonNumber::S4;
    } else if (this->_rowFourButtons[1].isPressed()){
        internalValue = this->_rowFourButtons[0].getAnalogValue();
        if (internalValue>100 && internalValue<300)
            return buttonNumber::S5;
        if (internalValue>300 && internalValue<550)
            return buttonNumber::S6;
        if (internalValue>550 && internalValue<800)
            return buttonNumber::S7;
        if (internalValue>960)
            return buttonNumber::S8;
    } else if (this->_rowFourButtons[2].isPressed()) {
        internalValue = this->_rowFourButtons[2].getAnalogValue();
        if (internalValue>100 && internalValue<300)
            return buttonNumber::S9;
        if (internalValue>300 && internalValue<550)
            return buttonNumber::S10;
        if (internalValue>550 && internalValue<800)
            return buttonNumber::S11;
        if (internalValue>960)
            return buttonNumber::S12;
    } else if (this->_rowFourButtons[3].isPressed()) {
        internalValue = this->_rowFourButtons[3].getAnalogValue();
        if (internalValue>100 && internalValue<300)
            return buttonNumber::S13;
        if (internalValue>300 && internalValue<550)
            return buttonNumber::S14;
        if (internalValue>550 && internalValue<800)
            return buttonNumber::S15;
        if (internalValue>960)
            return buttonNumber::S16;
    }
    return buttonNumber::noBtn;
}

void Keypad::onPressK(buttonNumber btn, ButtonOnPressCallback callback) {
    PushButton* push;
    switch(btn)
    {
        case buttonNumber::S1 :
        case buttonNumber::S2 :
        case buttonNumber::S3 :
        case buttonNumber::S4 :
            push = &this->_rowFourButtons[0];
            push->onPress(callback);
            break;       // and exits the switch
        case buttonNumber::S5 :
        case buttonNumber::S6 :
        case buttonNumber::S7 :
        case buttonNumber::S8 :
            push = &this->_rowFourButtons[0];
            push->onPress(callback);
            break;       // and exits the switch
        case buttonNumber::S9 :
        case buttonNumber::S10 :
        case buttonNumber::S11 :
        case buttonNumber::S12 :
            push = &this->_rowFourButtons[0];
            push->onPress(callback);
            break;       // and exits the switch
        case buttonNumber::S13 :
        case buttonNumber::S14 :
        case buttonNumber::S15 :
        case buttonNumber::S16 :
            push = &this->_rowFourButtons[0];
            push->onPress(callback);
            break;       // and exits the switch
    }
}
CallbackAttachedResponse Keypad::onReleaseK(buttonNumber btn, ButtonOnEventCallback callback) const {
    return CallbackAttachedResponse::attNoMoreRoom;
}
CallbackAttachedResponse Keypad::onReleaseK(buttonNumber btn, uint16_t wait, ButtonOnEventCallback callback) const {
    return CallbackAttachedResponse::attNoMoreRoom;
}
CallbackAttachedResponse Keypad::onReleaseK(buttonNumber btn, uint16_t wait, uint16_t max_wait, ButtonOnEventCallback callback) const {
    return CallbackAttachedResponse::attNoMoreRoom;
}
CallbackAttachedResponse Keypad::onHoldK(buttonNumber btn, uint16_t duration, ButtonOnEventCallback callback) const {
    return CallbackAttachedResponse::attNoMoreRoom;
}
CallbackAttachedResponse Keypad::onHoldRepeatK(buttonNumber btn, uint16_t duration, uint16_t repeat_every, ButtonOnEventRepeatCallback callback) const {
    return CallbackAttachedResponse::attNoMoreRoom;
}

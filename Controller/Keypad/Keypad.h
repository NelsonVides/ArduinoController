/*
 * Keypad.h
 *  Created on: Dec 30, 2017
 *      Author: Nelson Vides
 */
/*
 * What do I need from this class?
 * I'll update it once every loop,
 *      Then read its values: which button, and for how long.
 *      Let's say we will return a 'PushButton' object
 *      that has type and length data fields.
 */

#ifndef CLASSES_KEYPAD_H_
#define CLASSES_KEYPAD_H_

#include <Arduino.h>
#include "AnalogPushButtons.h"
#include "InfoButton.h"

class Keypad {
public:
	Keypad(const int pin1, const int pin2, const int pin3, const int pin4);
	virtual ~Keypad();
	void updateKeys();

	/// These callbacks must implement an action for each button on each action.
    void onPress(uint8_t row, ButtonOnPressCallback callback);
    CallbackAttachedResponse onRelease(uint8_t row, ButtonOnEventCallback callback) const;
    CallbackAttachedResponse onRelease(uint8_t row, uint16_t, ButtonOnEventCallback callback) const;
    CallbackAttachedResponse onRelease(uint8_t row, uint16_t, uint16_t, ButtonOnEventCallback callback) const;
    CallbackAttachedResponse onHold(uint8_t row, uint16_t, ButtonOnEventCallback callback) const;
    CallbackAttachedResponse onHoldRepeat(uint8_t row, uint16_t, uint16_t, ButtonOnEventRepeatCallback callback) const;
    bool is(AnalogPushButtons &btn1, AnalogPushButtons &btn2) const;
    bool isPressed(uint8_t butt) const;

private:
	const int _pin1;
	const int _pin2;
	const int _pin3;
	const int _pin4;
	AnalogPushButtons _rowFourButtons[4];
};



Keypad::Keypad(const int pin1, const int pin2, const int pin3, const int pin4)
	: _pin1(pin1), _pin2(pin2), _pin3(pin3), _pin4(pin4),
	  _rowFourButtons({AnalogPushButtons(pin1),
                       AnalogPushButtons(pin2),
                       AnalogPushButtons(pin3),
                       AnalogPushButtons(pin4)
                     })
{}
Keypad::~Keypad() {}

void Keypad::updateKeys()
{
    for (AnalogPushButtons btn : this->_rowFourButtons) {
        btn.update();
    }
}


void Keypad::onPress(uint8_t row, ButtonOnPressCallback callback) {
    this->_rowFourButtons[row].onPress(callback);
}
CallbackAttachedResponse Keypad::onRelease(uint8_t row, ButtonOnEventCallback callback) const {
    return this->_rowFourButtons[row].onRelease(callback);
}
CallbackAttachedResponse Keypad::onRelease(uint8_t row, uint16_t wait, ButtonOnEventCallback callback) const {
    return this->_rowFourButtons[row].onRelease(wait, callback);
}
CallbackAttachedResponse Keypad::onRelease(uint8_t row, uint16_t wait, uint16_t max_wait, ButtonOnEventCallback callback) const {
    return this->_rowFourButtons[row].onRelease(wait, max_wait, callback);
}
CallbackAttachedResponse Keypad::onHold(uint8_t row, uint16_t duration, ButtonOnEventCallback callback) const {
    return this->_rowFourButtons[row].onHold(duration, callback);
}
CallbackAttachedResponse Keypad::onHoldRepeat(uint8_t row, uint16_t duration, uint16_t repeat_every, ButtonOnEventRepeatCallback callback) const {
    return this->_rowFourButtons[row].onHoldRepeat(duration, repeat_every, callback);
}

bool Keypad::is(AnalogPushButtons &btn1, AnalogPushButtons &btn2) const
{
    return (&btn1 == &btn2) && (btn1.getButtonNumber() == btn2.getButtonNumber());
}

bool Keypad::isPressed(uint8_t butt) const
{
    //Serial.println("checking if pressed");
    //Serial.println((butt-1) % 4);
    //Serial.println(this->_rowFourButtons[(butt-1)%4].getButtonNumber());
    return (this->_rowFourButtons[(butt-1) % 4].getButtonNumber() == butt);
}


#endif /* CLASSES_KEYPAD_H_ */

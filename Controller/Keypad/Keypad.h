/*
 * Keypad.h
 *  Created on: Jan 4, 2018
 *      Author: Nelson Vides
 */
/*
 * This class assumes four pins are being used.
 * This should be fairly easy to change afterwards anyway.
 */

#ifndef KEYPAD_KEYPAD_H_
#define KEYPAD_KEYPAD_H_

#include "PushButton.h"

enum class buttonNumber {
    noBtn,
    S1,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7,
    S8,
    S9,
    S10,
    S11,
    S12,
    S13,
    S14,
    S15,
    S16
};

class Keypad {
public:
    Keypad(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);

    void updateKeys();

    /// These callbacks must implement an action for each button on each action.
    void onPressK(buttonNumber btn, ButtonOnPressCallback callback);
    CallbackAttachedResponse onReleaseK(buttonNumber btn, ButtonOnEventCallback callback) const;
    CallbackAttachedResponse onReleaseK(buttonNumber btn, uint16_t, ButtonOnEventCallback callback) const;
    CallbackAttachedResponse onReleaseK(buttonNumber btn, uint16_t, uint16_t, ButtonOnEventCallback) const;
    CallbackAttachedResponse onHoldK(buttonNumber btn, uint16_t, ButtonOnEventCallback callback) const;
    CallbackAttachedResponse onHoldRepeatK(buttonNumber btn, uint16_t, uint16_t, ButtonOnEventRepeatCallback callback) const;
    buttonNumber whichButtonIsPressed() const;

private:
    PushButton _rowFourButtons[4];
};

#endif /* KEYPAD_KEYPAD_H_ */

/*
 * KeypadCallbacks.h
 *  Created on: Jan 5, 2018
 *      Author: Nelson Vides
 */

#ifndef KEYPADCALLBACKS_H_
#define KEYPADCALLBACKS_H_

#include "LCD/Views.h"

namespace buttonsMgmt {
    static PushButton button1 = PushButton(pins::btn1);
    static PushButton button2 = PushButton(pins::btn2);
    static PushButton button3 = PushButton(pins::btn3);
    static PushButton button4 = PushButton(pins::btn4);
    enum class allButtons {
        unKnown,
        S1,        S2,        S3,        S4,
        S5,        S6,        S7,        S8,
        S9,        S10,       S11,       S12,
        S13,       S14,       S15,       S16
    };

    allButtons printButtonNumber(PushButton& btn)
    {
        static int16_t theLast = 0;
        int16_t btnNum = btn.getAnalogValue();
        if (btnNum < BouncerConsts::NOISE_TOLL) {
            btnNum = theLast;
        } else {
            theLast = btnNum;
        }
        if (btn.is(button1)) {
            if (180 < btnNum && btnNum < 220) {
                Serial.print(F("Button 1 "));
                return allButtons::S1;
            }
            if (380 < btnNum && btnNum < 420) {
                Serial.print(F("Button 2 "));
                return allButtons::S2;
            }
            if (580 < btnNum && btnNum < 620) {
                Serial.print(F("Button 3 "));
                return allButtons::S3;
            }
            if (780 < btnNum && btnNum < 820) {
                Serial.print(F("Button 4 "));
                return allButtons::S4;
            }
        } else if (btn.is(button2)) {
            if (180 < btnNum && btnNum < 220) {
                Serial.print(F("Button 5 "));
                return allButtons::S5;
            }
            if (380 < btnNum && btnNum < 420) {
                Serial.print(F("Button 6 "));
                return allButtons::S6;
            }
            if (580 < btnNum && btnNum < 620) {
                Serial.print(F("Button 7 "));
                return allButtons::S7;
            }
            if (780 < btnNum && btnNum < 820) {
                Serial.print(F("Button 8 "));
                return allButtons::S8;
            }
        } else if (btn.is(button3)) {
            if (180 < btnNum && btnNum < 220) {
                Serial.print(F("Button 9 "));
                return allButtons::S9;
            }
            if (380 < btnNum && btnNum < 420) {
                Serial.print(F("Button 10 "));
                return allButtons::S10;
            }
            if (580 < btnNum && btnNum < 620) {
                Serial.print(F("Button 11 "));
                return allButtons::S11;
            }
            if (780 < btnNum && btnNum < 820) {
                Serial.print(F("Button 12 "));
                return allButtons::S12;
            }
        } else if (btn.is(button4)) {
            if (180 < btnNum && btnNum < 220) {
                Serial.print(F("Button 13 "));
                return allButtons::S13;
            }
            if (380 < btnNum && btnNum < 420) {
                Serial.print(F("Button 14 "));
                return allButtons::S14;
            }
            if (580 < btnNum && btnNum < 620) {
                Serial.print(F("Button 15 "));
                return allButtons::S15;
            }
            if (780 < btnNum && btnNum < 820) {
                Serial.print(F("Button 16 "));
                return allButtons::S16;
            }
        }
        Serial.print(F("Hmmm, no button was"));
        return allButtons::unKnown;
    }

    void switchPowerLCD()
    {
        static uint8_t _stateLCD = 255;
        if (_stateLCD == 255) {
            _stateLCD = 128;
        } else if (_stateLCD == 128) {
            _stateLCD = 8;
        } else if (_stateLCD == 8) {
            _stateLCD = 0;
        } else {
            _stateLCD = 255;
        }
        analogWrite(pins::lcdBckLight,_stateLCD);
    }

    void switchRelay() {
        static boolean _stateRelay = true;
        _stateRelay = !_stateRelay;
        digitalWrite(pins::relayCon, _stateRelay);
    }

    // btn is a reference to the button that fired the event. That means you can use the same event handler for many buttons
    void onButtonPressed(PushButton& btn)
    {
        allButtons bt = printButtonNumber(btn);
        if (bt == allButtons::S1) {
            switchPowerLCD();
        } else if (bt == allButtons::S13) {
            Views::ViewIntro();
        } else if (bt == allButtons::S16) {
            switchRelay();
        }
    }

    // duration reports back how long it has been since the button was originally pressed.
    // repeatCount tells us how many times this function has been called by this button.
    void onButtonHeld(PushButton& btn, uint16_t duration, uint16_t repeatCount)
    {
        Serial.print(F("button has been held for "));
        Serial.print(duration);
        Serial.print(F(" ms; this event has been fired "));
        Serial.print(repeatCount);
        Serial.println(F(" times"));
        allButtons bt = printButtonNumber(btn);
        if (bt == allButtons::S13) {
            Views::ViewIntro();
        }
    }

    // duration reports back the total time that the button was held down
    void onButtonReleased(PushButton& btn, uint16_t duration)
    {
        printButtonNumber(btn);
        Serial.print(F(" released after "));
        Serial.print(duration);
        Serial.println(F(" ms"));
        allButtons bt = printButtonNumber(btn);
        if (bt == allButtons::S13) {
            Views::ViewIntro();
        }
    }
}
#endif KEYPADCALLBACKS_H_

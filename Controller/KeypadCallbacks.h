/*
 * KeypadCallbacks.h
 *  Created on: Jan 5, 2018
 *      Author: Nelson Vides
 */

#ifndef KEYPADCALLBACKS_H_
#define KEYPADCALLBACKS_H_

namespace buttonsMgmt {
    enum class allButtons {
        unKnown,
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

    allButtons printButtonNumber(PushButton& btn)
    {
        buttonNumber btnNum = btn.getLastPressedButton();
        if (btn.is(button1)) {
            if (btnNum == buttonNumber::R1) {
                Serial.print("Button 1");
                return allButtons::S1;
            }
            if (btnNum == buttonNumber::R2) {
                Serial.print("Button 2");
                return allButtons::S2;
            }
            if (btnNum == buttonNumber::R3) {
                Serial.print("Button 3");
                return allButtons::S3;
            }
            if (btnNum == buttonNumber::R4) {
                Serial.print("Button 4");
                return allButtons::S4;
            }
        } else if (btn.is(button2)) {
            if (btnNum == buttonNumber::R1) {
                Serial.print("Button 5");
                return allButtons::S5;
            }
            if (btnNum == buttonNumber::R2) {
                Serial.print("Button 6");
                return allButtons::S6;
            }
            if (btnNum == buttonNumber::R3) {
                Serial.print("Button 7");
                return allButtons::S7;
            }
            if (btnNum == buttonNumber::R4) {
                Serial.print("Button 8");
                return allButtons::S8;
            }
        } else if (btn.is(button3)) {
            if (btnNum == buttonNumber::R1) {
                Serial.print("Button 9");
                return allButtons::S9;
            }
            if (btnNum == buttonNumber::R2) {
                Serial.print("Button 10");
                return allButtons::S10;
            }
            if (btnNum == buttonNumber::R3) {
                Serial.print("Button 11");
                return allButtons::S11;
            }
            if (btnNum == buttonNumber::R4) {
                Serial.print("Button 12");
                return allButtons::S12;
            }
        } else if (btn.is(button4)) {
            if (btnNum == buttonNumber::R1) {
                Serial.print("Button 13");
                return allButtons::S13;
            }
            if (btnNum == buttonNumber::R2) {
                Serial.print("Button 14");
                return allButtons::S14;
            }
            if (btnNum == buttonNumber::R3) {
                Serial.print("Button 15");
                return allButtons::S15;
            }
            if (btnNum == buttonNumber::R4) {
                Serial.print("Button 16");
                return allButtons::S16;
            }
        } else {
            Serial.print("Hmmm, no button was");
        }
        return allButtons::unKnown;
    }

    // btn is a reference to the button that fired the event. That means you can use the same event handler for many buttons
    void onButtonPressed(PushButton& btn)
    {
        allButtons bt = printButtonNumber(btn);
        Serial.println(" pressed");
        if (bt == allButtons::S1) {
            static bool _stateLCD = true;
            _stateLCD = !_stateLCD;
            digitalWrite(8, _stateLCD);
        }
    }

    // duration reports back how long it has been since the button was originally pressed.
    // repeatCount tells us how many times this function has been called by this button.
    void onButtonHeld(PushButton& btn, uint16_t duration, uint16_t repeatCount)
    {
        Serial.print("button has been held for ");
        Serial.print(duration);
        Serial.print(" ms; this event has been fired ");
        Serial.print(repeatCount);
        Serial.println(" times");
    }

    // duration reports back the total time that the button was held down
    void onButtonReleased(PushButton& btn, uint16_t duration)
    {
        printButtonNumber(btn);
        Serial.print(" released after ");
        Serial.print(duration);
        Serial.println(" ms");
    }
}
#endif KEYPADCALLBACKS_H_

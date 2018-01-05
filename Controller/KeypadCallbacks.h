/*
 * KeypadCallbacks.h
 *  Created on: Jan 5, 2018
 *      Author: Nelson Vides
 */

#ifndef KEYPADCALLBACKS_H_
#define KEYPADCALLBACKS_H_

void printButtonNumber(PushButton& btn)
{
    buttonNumber btnNum = btn.getLastPressedButton();
    if (btn.is(button1)) {
        if (btnNum == buttonNumber::R1)
            Serial.print("Button 1");
        if (btnNum == buttonNumber::R2)
            Serial.print("Button 2");
        if (btnNum == buttonNumber::R3)
            Serial.print("Button 3");
        if (btnNum == buttonNumber::R4)
            Serial.print("Button 4");
    } else if (btn.is(button2)) {
        if (btnNum == buttonNumber::R1)
            Serial.print("Button 5");
        if (btnNum == buttonNumber::R2)
            Serial.print("Button 6");
        if (btnNum == buttonNumber::R3)
            Serial.print("Button 7");
        if (btnNum == buttonNumber::R4)
            Serial.print("Button 8");
    } else if (btn.is(button3)) {
        if (btnNum == buttonNumber::R1)
            Serial.print("Button 9");
        if (btnNum == buttonNumber::R2)
            Serial.print("Button 10");
        if (btnNum == buttonNumber::R3)
            Serial.print("Button 11");
        if (btnNum == buttonNumber::R4)
            Serial.print("Button 12");
    } else if (btn.is(button4)) {
        if (btnNum == buttonNumber::R1)
            Serial.print("Button 13");
        if (btnNum == buttonNumber::R2)
            Serial.print("Button 14");
        if (btnNum == buttonNumber::R3)
            Serial.print("Button 15");
        if (btnNum == buttonNumber::R4)
            Serial.print("Button 16");
    } else {
        Serial.print("Hmmm, no button was");
    }
}

// btn is a reference to the button that fired the event. That means you can use the same event handler for many buttons
void onButtonPressed(PushButton& btn){
    printButtonNumber(btn);
    Serial.println(" pressed");
}

// duration reports back how long it has been since the button was originally pressed.
// repeatCount tells us how many times this function has been called by this button.
void onButtonHeld(PushButton& btn, uint16_t duration, uint16_t repeatCount){
        Serial.print("button has been held for ");
        Serial.print(duration);
        Serial.print(" ms; this event has been fired ");
        Serial.print(repeatCount);
        Serial.println(" times");
}

// duration reports back the total time that the button was held down
void onButtonReleased(PushButton& btn, uint16_t duration){
    printButtonNumber(btn);
    Serial.print(" released after ");
    Serial.print(duration);
    Serial.println(" ms");
}

#endif KEYPADCALLBACKS_H_

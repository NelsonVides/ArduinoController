#include <Arduino.h>
#include <LiquidCrystal.h>

#include "classes/Thermometer.h"
#include "Keypad/PushButton.h"

namespace thermoMgmt {
    constexpr uint8_t trPin = A0;
    Thermometer Therm(trPin);
}

namespace LCDMgmt {
    constexpr uint8_t rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
    LiquidCrystal Lcd(rs, en, d4, d5, d6, d7);
}

namespace buttonsMgmt {
    PushButton button1 = PushButton(A1);
    PushButton button2 = PushButton(A2);
    PushButton button3 = PushButton(A3);
    PushButton button4 = PushButton(A4);
    void onButtonPressed(PushButton& btn);
    void onButtonHeld(PushButton& btn, uint16_t duration, uint16_t repeatCount);
    void onButtonReleased(PushButton& btn, uint16_t duration);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("INIT of everything");

    LCDMgmt::Lcd.begin(16, 2);
    LCDMgmt::Lcd.clear();
    LCDMgmt::Lcd.print("Arduino");
    LCDMgmt::Lcd.setCursor(2, 1);
    LCDMgmt::Lcd.print("Celcius ");
    LCDMgmt::Lcd.setCursor(10, 1);

    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);

    // When the button is first pressed, call the function onButtonPressed (further down the page)
    buttonsMgmt::button1.onPress(buttonsMgmt::onButtonPressed);
    buttonsMgmt::button2.onPress(buttonsMgmt::onButtonPressed);
    buttonsMgmt::button3.onPress(buttonsMgmt::onButtonPressed);
    buttonsMgmt::button4.onPress(buttonsMgmt::onButtonPressed);

    // Once the button has been held for 1 second (1000ms) call onButtonHeld. Call it again every 0.5s (500ms) until it is let go
    buttonsMgmt::button1.onHoldRepeat(1000, 500, buttonsMgmt::onButtonHeld);
    // When the button is released, call onButtonReleased
    buttonsMgmt::button1.onRelease(buttonsMgmt::onButtonReleased);
    buttonsMgmt::button2.onRelease(buttonsMgmt::onButtonReleased);
    buttonsMgmt::button3.onRelease(buttonsMgmt::onButtonReleased);
    buttonsMgmt::button4.onRelease(buttonsMgmt::onButtonReleased);
}

void loop()
{
    if (thermoMgmt::Therm.isTime()) {
        LCDMgmt::Lcd.setCursor(10, 1);
        LCDMgmt::Lcd.print(thermoMgmt::Therm.getCelsius());
    }

    // Update those buttons
    buttonsMgmt::button1.update();
    buttonsMgmt::button2.update();
    buttonsMgmt::button3.update();
    buttonsMgmt::button4.update();
}

#include "KeypadCallbacks.h"

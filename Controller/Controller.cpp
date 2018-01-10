#include <Arduino.h>
#include <Wire.h>

#include "LCD/LiquidCrystal_I2C.h"
#include "classes/Thermometer.h"
#include "Keypad/PushButton.h"

/*
 * TODO's:
 *      implement LCD backlight dimming
 *      new thermometer with its other forecasting
 *      connect the GSM and program it
 *      radio receiver and tinyBrd
 */

namespace pins {
    constexpr uint8_t btn1 = A0;
    constexpr uint8_t btn2 = A1;
    constexpr uint8_t btn3 = A2;
    constexpr uint8_t btn4 = A3;
    constexpr uint8_t lcdBckLight = 3; //TODO: implement backlight dimming. Three levels for example (on/mid/off)

}

namespace thermoMgmt {
    //constexpr uint8_t trPin = A0;
    //static Thermometer Therm(trPin);
}

namespace LCDMgmt {
    LiquidCrystal_I2C Lcd(0x27, 2, 1, 0, 4, 5, 6, 7, pins::lcdBckLight, t_backlightPol::POSITIVE);
}

namespace buttonsMgmt {
    static PushButton button1 = PushButton(pins::btn1);
    static PushButton button2 = PushButton(pins::btn2);
    static PushButton button3 = PushButton(pins::btn3);
    static PushButton button4 = PushButton(pins::btn4);
    void onButtonPressed(PushButton& btn);
    void onButtonHeld(PushButton& btn, uint16_t duration, uint16_t repeatCount);
    void onButtonReleased(PushButton& btn, uint16_t duration);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("INIT of everything");

    LCDMgmt::Lcd.begin(16, 2, LCD_5x8DOTS);
    LCDMgmt::Lcd.clear();
    LCDMgmt::Lcd.print("Arduino");
    LCDMgmt::Lcd.setCursor(2, 1);
    LCDMgmt::Lcd.print("Celcius ");
    LCDMgmt::Lcd.setCursor(10, 1);

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
    //if (thermoMgmt::Therm.isTime()) {
        //LCDMgmt::Lcd.setCursor(10, 1);
        //LCDMgmt::Lcd.print(thermoMgmt::Therm.getCelsius());
    //}

    // Update those buttons
    buttonsMgmt::button1.update();
    buttonsMgmt::button2.update();
    buttonsMgmt::button3.update();
    buttonsMgmt::button4.update();
}

#include "KeypadCallbacks.h"

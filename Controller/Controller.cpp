#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include <Chronos.h>

#include "LCD/LiquidCrystal_I2C.h"
#include "Keypad/PushButton.h"
#include "Weather/BME280I2C.h"
//#include "Chronos/TimeProvider.h"
/* TODO's:
 *      going well: new thermometer with its other forecasting
 *      next! connect the GSM and program it
 *      Calendars
 *      radio receiver and tinyBrd
 */

namespace pins {
    constexpr auto SerialBaudRate = 115200;
    constexpr uint8_t btn1 = A0;
    constexpr uint8_t btn2 = A1;
    constexpr uint8_t btn3 = A2;
    constexpr uint8_t btn4 = A3;
    constexpr uint8_t lcdBckLight = 3;
}

#include "KeypadCallbacks.h"

namespace Weather {
    Weather::BME280I2C::Settings SettingsBME(
            Weather::BME280I2C::OSR::OSR_X1,
            Weather::BME280I2C::OSR::OSR_X1,
            Weather::BME280I2C::OSR::OSR_X1,
            Weather::BME280I2C::Mode::Mode_Forced,
            Weather::BME280I2C::StandbyTime::StandbyTime_1000ms,
            Weather::BME280I2C::Filter::Filter_Off,
            Weather::BME280I2C::SpiEnable::SpiEnable_False,
            Weather::BME280I2C::I2CAddr_0x76);
    static Weather::BME280I2C Therm(SettingsBME);
}

namespace LCDMgmt {
    LiquidCrystal::LiquidCrystal_I2C Lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, LiquidCrystal::t_backlightPol::POSITIVE);
}

namespace timeMgmt {
    unsigned long interval = 10000;
    bool isTime() {
        static unsigned long _previousMillis;
        unsigned long currentMillis = millis();
        if (currentMillis - _previousMillis >= interval) {
            _previousMillis = currentMillis;
            return true;
        }
        return false;
    }
}

void setup()
{
    Serial.begin(pins::SerialBaudRate);
    Wire.begin();

    Serial.println("INIT of everything");

    LCDMgmt::Lcd.begin(16, 2, LiquidCrystal::LCD_5x8DOTS);
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
    pinMode(pins::lcdBckLight, OUTPUT);
    analogWrite(pins::lcdBckLight, HIGH);

    while(!Weather::Therm.begin()) {
        delay(1000);
    }
}

void loop()
{
    if (timeMgmt::isTime()) {
        LCDMgmt::Lcd.setCursor(10, 1);
        float temp(NAN), hum(NAN), pres(NAN);

        Weather::Therm.read(pres, temp, hum,
                Weather::BME280::TempUnit::TempUnit_Celsius,
                Weather::BME280::PresUnit::PresUnit_hPa);
        LCDMgmt::Lcd.print(temp);

        Serial.print("Temp: ");
        Serial.print(temp);
        Serial.print("°C");
        Serial.print("\t\tHumidity: ");
        Serial.print(hum);
        Serial.print("% RH");
        Serial.print("\t\tPressure: ");
        Serial.print(pres);
        Serial.println("Pa");
    }

    // Update those buttons
    buttonsMgmt::button1.update();
    buttonsMgmt::button2.update();
    buttonsMgmt::button3.update();
    buttonsMgmt::button4.update();
}

#include "KeypadCallbacks.h"

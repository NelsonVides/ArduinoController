#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include <Chronos.h>

#include "AirQuality/MQ135.h" //TODO: do something with all that floating point binary boilerplate! :(
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

    constexpr uint8_t relayCon = 2;

    constexpr uint8_t lcdBckLight = 5;

    constexpr uint8_t smogSensor = A2;
    constexpr uint8_t smogSwitch = 2;
    //TODO: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

    constexpr uint8_t simRST = 6;
    constexpr uint8_t simRX = 7;
    constexpr uint8_t simTX = 8;

    constexpr uint8_t radioCEN = 9;
    constexpr uint8_t radioCS = 10;
    constexpr uint8_t radioMOSI = 11;
    constexpr uint8_t radioMISO = 12;
    constexpr uint8_t radioSCK = 13;

}

#include "LCD/Views.h"
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

namespace Air {
    MQ135 Smog = MQ135(pins::smogSensor);
}

namespace timeMgmt {
    unsigned long interval = 1000;
    bool isTime()
    {
        static unsigned long _previousMillis;
        unsigned long currentMillis = millis();
        if (currentMillis - _previousMillis >= interval) {
            _previousMillis = currentMillis;
            return true;
        }
        return false;
    }
}

namespace SIM {
    //Create software serial object to communicate with SIM800
    SoftwareSerial sim(pins::simTX, pins::simRX);
}

void setup()
{
    Serial.begin(pins::SerialBaudRate);

    //SIM card
    SIM::sim.begin(9600);
    delay(1000);

    ///WEATHER
    Wire.begin();
    while (!Weather::Therm.begin()) {
        delay(1000);
    }

    ///BUTTONS
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

    // Relay
    pinMode(pins::relayCon, OUTPUT);

    // LCD
    Views::Lcd.begin(16, 2, LiquidCrystal::LCD_5x8DOTS);
    Views::ViewIntro();
    pinMode(pins::lcdBckLight, OUTPUT);
    analogWrite(pins::lcdBckLight, HIGH);

    Serial.println(F("INIT of everything"));
}

void loop()
{
    if (timeMgmt::isTime()) {
        const Weather::Measures res = Weather::Therm.read(
                Weather::BME280::TempUnit::TempUnit_Celsius,
                Weather::BME280::PresUnit::PresUnit_hPa);
        Views::ViewWeather(res);

        Serial.print(res.temperature);
        Serial.print(F("°C"));
        Serial.print(F("\t\tHumidity: "));
        Serial.print(res.humidity);
        Serial.print(F("% RH"));
        Serial.print(F("\t\tPressure: "));
        Serial.print(res.pressure);
        Serial.println(F("Pa"));
    }

    // Update those buttons
    buttonsMgmt::button1.update();
    buttonsMgmt::button2.update();
    buttonsMgmt::button3.update();
    buttonsMgmt::button4.update();
}

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Time.h>
#include <Chronos.h>

#include "AirQuality/MQ135.h"
#include "LCD/LiquidCrystal_I2C.h"
#include "Keypad/PushButton.h"
#include "RelayControl/Relay.h"

/* TODO's:
 *      going well: new thermometer with its other forecasting
 *      do something with all that floating point binary boilerplate! :(
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
    constexpr uint8_t dallas = 49;

    constexpr uint8_t smogSensor = A2;
    constexpr uint8_t smogSwitch = 2;
    //TODO: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

    constexpr uint8_t simRST = 6;
    constexpr uint8_t simRX = 7;
    constexpr uint8_t simTX = 8;

    constexpr uint8_t radioIRQ  = 19;
    constexpr uint8_t radioCEN  = 48;
    constexpr uint8_t radioMISO = 50;
    constexpr uint8_t radioMOSI = 51;
    constexpr uint8_t radioSCK  = 52;
    constexpr uint8_t radioCS   = 53;
}

#include "LCD/Views.h"
#include "KeypadCallbacks.h"

namespace Weather {
    OneWire oneWire(pins::dallas);
    DallasTemperature sensors(&oneWire);
}

namespace Air {
    MQ135 Smog = MQ135(pins::smogSensor);
}

namespace Heater {
    Relay relay(pins::relayCon);
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
    delay(100);

    //SIM card
    SIM::sim.begin(9600);
    delay(100);

    ///WEATHER
    Weather::sensors.begin();
    delay(100);

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
    Heater::relay.begin();

    ///SMOG
    Air::Smog.begin();

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
        Weather::sensors.requestTemperatures();
        Serial.print("Temperature is: ");
        Serial.println(Weather::sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
    }

    // Update those buttons
    //buttonsMgmt::button1.update();
    //buttonsMgmt::button2.update();
    //buttonsMgmt::button3.update();
    //buttonsMgmt::button4.update();
}

#include <avr/wdt.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LowPower.h>
#include <RF24.h>
#include <RGBLED.hpp>
#include "MQ135.h" //TODO: do something with all that floating point binary boilerplate! :(
#include "Weather/BME280I2C.h"

/*TODO's
 * Implement WatchDog and Sleeps to save energy.
 */

namespace pins {
    constexpr auto SerialBaudRate = 115200;

    constexpr auto wakeupInterrupt = 2;

    constexpr uint8_t photoSensor = A0;
    constexpr uint8_t smogSensor = A1;
    //TODO: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

    constexpr uint8_t redLED = 3;
    constexpr uint8_t greenLED = 5;
    constexpr uint8_t blueLED = 6;

    constexpr uint8_t radioCEN = 9;
    constexpr uint8_t radioCS = 10;
    constexpr uint8_t radioMOSI = 11;
    constexpr uint8_t radioMISO = 12;
    constexpr uint8_t radioSCK = 13;
    constexpr uint8_t addresses[2][6] = {"000001","000002"};
}

namespace Air {
    MQ135 Smog = MQ135(pins::smogSensor);
}

namespace Radio {
    RF24 Trasmitter(pins::radioCEN, pins::radioCS);
}

namespace Led {
    RGBLED rgbLed(pins::redLED, pins::greenLED, pins::blueLED, CommonElectrode::cathode);

    uint8_t mapToByte(uint16_t val, uint8_t inMin, uint8_t inMax)
    {
        return (255 * (val - inMin)) / (inMax - inMin);
    }

    void setLED(const Weather::Measures& measures, float smog, uint16_t ambient)
    {
        if (smog > 1000.0) { // full white
            rgbLed.brightness(255);
            rgbLed(255, 255, 255);
        } else if (measures.temperature <= -20.0) { // full blue
            rgbLed.brightness(255);
            rgbLed(0, 0, 255);
        } else if (measures.temperature >= 40.0) { // full red
            rgbLed.brightness(255);
            rgbLed(255, 0, 0);
        } else {
            uint8_t redFactor = mapToByte((uint16_t)measures.temperature, -20,40);
            uint8_t blueFactor = mapToByte((uint16_t)measures.temperature, 40,-20);
            uint8_t greenFactor = mapToByte(smog, 0,1023);
            rgbLed.brightness(map(ambient,0,200, 10, 255));
            rgbLed(redFactor, blueFactor, greenFactor);
        }
    }
}

namespace Weather {
    Weather::BME280I2C::Settings SettingsBME(Weather::BME280I2C::OSR::OSR_X1,
            Weather::BME280I2C::OSR::OSR_X1, Weather::BME280I2C::OSR::OSR_X1,
            Weather::BME280I2C::Mode::Mode_Forced,
            Weather::BME280I2C::StandbyTime::StandbyTime_1000ms,
            Weather::BME280I2C::Filter::Filter_Off,
            Weather::BME280I2C::SpiEnable::SpiEnable_False,
            Weather::BME280I2C::I2CAddr_0x76);
    static Weather::BME280I2C Therm(SettingsBME);
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

void setup()
{
    Serial.begin(pins::SerialBaudRate);

    ///RADIO
    Radio::Trasmitter.begin();
    Radio::Trasmitter.openWritingPipe(pins::addresses[0]); // 00002
    Radio::Trasmitter.openReadingPipe(1, pins::addresses[1]); // 00001
    Radio::Trasmitter.setPALevel(RF24_PA_MIN);

    ///WEATHER
    Wire.begin();
    while (!Weather::Therm.begin()) {
        Serial.println(F("Got it"));
        delay(1000);
    }

    Serial.println(F("INIT of everything"));
}

void loop()
{
    //  wdt_enable(WDTO_8S);
    if (timeMgmt::isTime()) {

        const Weather::Measures res = Weather::Therm.read(
                                      Weather::BME280::TempUnit::TempUnit_Celsius,
                                      Weather::BME280::PresUnit::PresUnit_hPa);

        const uint16_t photoValue = analogRead(pins::photoSensor);
        const float smog = Air::Smog.getCorrectedPPM(res.temperature, res.humidity);

        Led::setLED(res, smog, photoValue);
        Led::rgbLed.show();

        ///Serial debug stuff
        Serial.print(F("smog: "));
        Serial.print(smog);
        Serial.print(F(" gaz"));

        Serial.print(F("\tAmbient light: "));
        Serial.print(photoValue);
        Serial.print(F(" lumens"));

        Serial.print(F(" \tTemperature: "));
        Serial.print(res.temperature);
        Serial.print(F(" °C"));

        Serial.print(F(" \tHumidity: "));
        Serial.print(res.humidity);
        Serial.print(F(" % RH"));

        Serial.print(F("\tPressure: "));
        Serial.print(res.pressure);
        Serial.println(F(" Pa"));
    }
}

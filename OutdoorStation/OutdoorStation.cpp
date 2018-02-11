#include <avr/wdt.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <LowPower.h>
#include "MQ135.h" //TODO: do something with all that floating point binary boilerplate! :(
#include "Weather/BME280I2C.h"

//Measure Battery>
#include <VoltageReference.h>
#include <Battery.h>

/*TODO's
 * Implement WatchDog and Sleeps to save energy.
 */

namespace pins {
    constexpr auto SerialBaudRate = 115200;

    constexpr auto wakeupInterrupt = 2;

    constexpr uint8_t photoSensor = A0;
    constexpr uint8_t smogSensor = A1;
    //TODO: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library
    constexpr uint8_t powerSensor = A2;
    constexpr uint8_t powerActivator = 2;

    constexpr uint8_t redLED = 3;
    constexpr uint8_t greenLED = 6;
    constexpr uint8_t blueLED = 5;

    constexpr uint8_t radioCEN = 9;
    constexpr uint8_t radioCS = 10;
    constexpr uint8_t radioMOSI = 11;
    constexpr uint8_t radioMISO = 12;
    constexpr uint8_t radioSCK = 13;
    constexpr uint8_t addresses[2][6] = {"000001","000002"};
}

namespace Power {
    constexpr float ratio = 22;
    //Battery bat(3500, 2500, pins::powerSensor, pins::powerActivator);
}

namespace Air {
    MQ135 Smog = MQ135(pins::smogSensor);
}

namespace Radio {
    RF24 Trasmitter(pins::radioCEN, pins::radioCS);
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
        delay(1000);
    }

    ///SMOG
    Air::Smog.begin();

    Serial.println(F("INIT of everything"));
}

class Payload {
public:
    float pressure;
    float humidity;
    float temperature;
    uint16_t photoValue;
    float smog;
};

void loop()
{
    //  wdt_enable(WDTO_8S);
    if (timeMgmt::isTime()) {
        Payload load;

        Weather::Therm.read(load.pressure, load.temperature, load.humidity,
                Weather::BME280::TempUnit::TempUnit_Celsius,
                Weather::BME280::PresUnit::PresUnit_hPa);

        load.smog = Air::Smog.getCalibratedCO(load.temperature, load.humidity);
        load.photoValue = analogRead(pins::photoSensor);

        ///Serial debug stuff
        Serial.print(F("smog: "));
        Serial.print(load.smog);
        Serial.print(F(" gaz"));

        Serial.print(F("\tAmbient light: "));
        Serial.print(load.photoValue);
        Serial.print(F(" lumens"));

        Serial.print(F(" \tTemperature: "));
        Serial.print(load.temperature);
        Serial.print(F(" °C"));

        Serial.print(F(" \tHumidity: "));
        Serial.print(load.humidity);
        Serial.print(F(" % RH"));

        Serial.print(F("\tPressure: "));
        Serial.print(load.pressure);
        Serial.println(F(" Pa"));
    }
}

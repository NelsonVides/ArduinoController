#include <avr/wdt.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LowPower.h>
#include "AirQuality/MQ135.h" //TODO: do something with all that floating point binary boilerplate! :(
#include "Weather/BME280I2C.h"
#include "photoResistor/LightDependentResistor.h"
#include "Battery/VoltageReference.h"

/*TODO's
 * Implement WatchDog and Sleeps to save energy.
 */

namespace pins {
    constexpr auto SerialBaudRate = 115200;

    constexpr uint8_t photoSensor = A0;

    constexpr uint8_t smogSensor = A2;
    constexpr uint8_t smogSwitch = 2;
    //TODO: https://hackaday.io/project/3475-sniffing-trinket/log/12363-mq135-arduino-library

    constexpr uint8_t radioCEN = 9;
    constexpr uint8_t radioCS = 10;
    constexpr uint8_t radioMOSI = 11;
    constexpr uint8_t radioMISO = 12;
    constexpr uint8_t radioSCK = 13;
}

namespace Power {
    VoltageReference vRef;
}

namespace Air {
    MQ135 Smog = MQ135(pins::smogSensor);
}

namespace Radio {
    constexpr uint64_t writingPipe = 0xB00B1E5000LL;
    RF24 Trasmitter(pins::radioCEN, pins::radioCS);
}

namespace Light {
    constexpr uint16_t pullDown = 988;
    LightDependentResistor photoCell(
            pins::photoSensor,
            pullDown,
            LightDependentResistor::ePhotoCellKind::GL5537_1,
            false);
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
    Serial.println(F("Serial started. On setup."));

    ///RADIO
    Radio::Trasmitter.begin();
    Radio::Trasmitter.setAutoAck(true);
    Radio::Trasmitter.enableAckPayload();
    Radio::Trasmitter.setRetries(4,3);
    Radio::Trasmitter.openWritingPipe(Radio::writingPipe);
    Radio::Trasmitter.stopListening();
    Radio::Trasmitter.setPALevel(RF24_PA_MIN);

    ///WEATHER
    Wire.begin();
    while (!Weather::Therm.begin()) {
        delay(1000);
    }

    ///SMOG
    Air::Smog.begin();

    ///BATTERY
    Power::vRef.begin(1093905);

    Serial.println(F("INIT of everything"));
}

class Payload {
public:
    float pressure;
    float temperature;
    float humidity;
    float smog;
    uint16_t photoValue;
    uint16_t batteryLoad;
};


inline void sleepMinutes(uint8_t min)
{
    for (int i = 0; i < ((uint8_t)(7.5 * min)); i++) {
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }
}


void loop()
{
    //sleepMinutes(15);

    float resistance = Air::Smog.getResistance();//resistance
    Air::Smog.getRZero();

    if (timeMgmt::isTime()) {
        Radio::Trasmitter.powerUp();
        Radio::Trasmitter.stopListening();

        static Payload payload;
        Weather::Therm.read(payload.pressure, payload.temperature, payload.humidity,
                Weather::BME280::TempUnit::TempUnit_Celsius,
                Weather::BME280::PresUnit::PresUnit_hPa);

        digitalWrite(pins::smogSwitch,HIGH);
        delay(50);
        Serial.println(Air::Smog.getRZero());
        //payload.smog = Air::Smog.getCalibratedCO(payload.temperature, payload.humidity);
        payload.smog = Air::Smog.getCO(resistance);
        digitalWrite(pins::smogSwitch,LOW);

        payload.photoValue = Light::photoCell.getCurrentLux();

        payload.batteryLoad = Power::vRef.readVcc();

        bool ok = Radio::Trasmitter.write(&payload, sizeof(payload));
        //Serial.println(ok);
        delay(80);
        Radio::Trasmitter.powerDown();
        delay(20);

        ///Serial debug stuff
        Serial.print(F("Battery: "));
        Serial.print(payload.batteryLoad);
        Serial.print(F(" V"));

        Serial.print(F("\tSmog: "));
        Serial.print(payload.smog);
        Serial.print(F(" gaz"));

        Serial.print(F("\tAmbient light: "));
        Serial.print(payload.photoValue);
        Serial.print(F(" luxs"));

        Serial.print(F(" \tTemperature: "));
        Serial.print(payload.temperature);
        Serial.print(F(" °C"));

        Serial.print(F(" \tHumidity: "));
        Serial.print(payload.humidity);
        Serial.print(F(" % RH"));

        Serial.print(F("\tPressure: "));
        Serial.print(payload.pressure);
        Serial.println(F(" Pa"));
    }
}

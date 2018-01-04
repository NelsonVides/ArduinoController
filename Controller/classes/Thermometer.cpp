/*
 * Thermometer.cpp
 *  Created on: Jan 4, 2018
 *      Author: Nelson Vides
 */

#include "Thermometer.h"

namespace {
    constexpr unsigned long interval = 1000;     // interval at which to blink (milliseconds)
}

Thermometer::Thermometer(uint8_t pin) :
    _pin(pin)
{
    pinMode(pin,INPUT);
};

float Thermometer::getCelsius() const
{
    return (this->getVoltage() - 32.0) * (5.0 / 9.0);
}

float Thermometer::getFarenheit() const
{
    return this->getVoltage();
}

bool Thermometer::isTime()
{
    unsigned long currentMillis = millis();
    if (currentMillis - _previous_millis >= interval) {
        _previous_millis = currentMillis;
        return true;
    }
    return false;
}

inline float Thermometer::getVoltage() const
{
    return (analogRead(this->_pin) * 500.0) / 1023.0;
}

/*
 * Thermometer.cpp
 *  Created on: Jan 4, 2018
 *      Author: Nelson Vides
 */

#include "Thermometer.h"

namespace {
    constexpr float adjuster = 1023.0 / 5.0;
    constexpr float multiplier = 100.0;
    constexpr float celsiometer = 50.0;
    constexpr unsigned long interval = 1000; //interval at which to blink (milliseconds)
}

Thermometer::Thermometer(uint8_t pin) :
    _pin(pin)
{
    pinMode(pin,INPUT);
};

float Thermometer::getCelsius() const
{
    return (this->getVoltage() * multiplier) - celsiometer;
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
    return analogRead(this->_pin) / adjuster;
}

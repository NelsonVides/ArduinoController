/*
 * Relay.cpp
 *
 *  Created on: Feb 22, 2018
 *      Author: Nelson Vides
 */

#include "Relay.h"

using namespace Heater;

Relay::Relay(uint8_t pin) : _pin(pin) {}

void Relay::begin() const
{
    pinMode(this->_pin,OUTPUT);
    digitalWrite(this->_pin, LOW);
}

void Relay::connect() const
{
    digitalWrite(this->_pin, HIGH);
}

void Relay::disconnect() const
{
    digitalWrite(this->_pin, LOW);
}



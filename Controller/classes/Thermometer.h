/*
 * Thermometer.h
 *  Created on: Jan 4, 2018
 *      Author: Nelson Vides
 */

#ifndef CLASSES_THERMOMETER_H_
#define CLASSES_THERMOMETER_H_

#include <Arduino.h>

namespace ThermSpace {

}

class Thermometer {
public:
    explicit Thermometer(uint8_t pin);
    float getCelsius() const;
    float getFarenheit() const;
    bool isTime();

private:
    const uint8_t _pin;
    unsigned long _previous_millis = 0;
    inline float getVoltage() const;
};

#endif /* CLASSES_THERMOMETER_H_ */

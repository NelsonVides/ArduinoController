/*
 * Thermo.h
 *
 *  Created on: Dec 28, 2017
 *      Author: plZZ0100
 */

#ifndef CLASSES_THERMO_H_
#define CLASSES_THERMO_H_

#include <Arduino.h>

class Thermo {
public:
	explicit Thermo(int pin);
	virtual ~Thermo() {};
	inline float getCelsius();
	inline float getFarenheit();
private:
	const int sensor;
	inline float getVoltage();
	inline int readSensor();
};

Thermo::Thermo(int pin):sensor(pin) {
	pinMode(pin,INPUT);
};

inline float Thermo::getCelsius() {
	return (this->getVoltage() - 32.0) * (5.0 / 9.0);
}

inline float Thermo::getFarenheit() {
	return this->getVoltage();
}

inline float Thermo::getVoltage() {
	return (this->readSensor() * 500.0) / 1023.0;
}

inline int Thermo::readSensor() {
	return analogRead(this->sensor);
}

#endif /* CLASSES_THERMO_H_ */

#ifndef CLASSES_BUTTONSCONTROLS_H_
#define CLASSES_BUTTONSCONTROLS_H_

#include <Arduino.h>

class ButtonsControls {

public:
	explicit ButtonsControls(int pin);
	virtual ~ButtonsControls();
	inline void updateState();

	inline const bool getState() const;
	inline const int getType() const;

private:
	const int _pin;
	int _type = 0;
	int _state = 0;
	inline const int readSensor() const;

	//unsigned long previousMillis = 0;        // will store last time LED was updated
	//static constexpr long interval = 100;           // interval at which to blink (milliseconds)
};




ButtonsControls::ButtonsControls(int pin) : _pin(pin)
{
	pinMode(pin, INPUT);
}

ButtonsControls::~ButtonsControls() {}

inline void ButtonsControls::updateState()
{
	//Serial.println("updating state");
	int pressLength = 0;
	while (this->readSensor() == HIGH) {

		//unsigned long currentMillis = millis();
		//if (currentMillis - previousMillis >= interval) {
		//}
		delay(100); //TODO: get rid of this delay
		pressLength += 100;
		Serial.println(pressLength);
	}
	if (pressLength <= 800) {
		this->_type = 0;
	} else {
		this->_type = 1;
	}
	if (pressLength > 0) {
		this->_state = !this->_state;
	}
}

inline const bool ButtonsControls::getState() const
{
	return this->_state;
}

inline const int ButtonsControls::getType() const
{
	return this->_type;
}

inline const int ButtonsControls::readSensor() const
{
	return digitalRead(this->_pin);
}

#endif /* CLASSES_BUTTONSCONTROLS_H_ */

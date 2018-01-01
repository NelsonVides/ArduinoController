#ifndef KEYPAD_INFOBUTTON_H_
#define KEYPAD_INFOBUTTON_H_

#include <inttypes.h>
#include <Arduino.h>

class InfoButton {
public:
	InfoButton();
	virtual ~InfoButton();

	int getNumber() const;
	int getLength() const;

	void setNumber(const int Number);
	void setLength(const int Length);

private:
	int _number = 0;
	int _length = 0;
};



InfoButton::InfoButton(){}
InfoButton::~InfoButton() {}

int InfoButton::getNumber() const {
    return this->_length;
}

int InfoButton::getLength() const {
    return this->_number;
}

void InfoButton::setNumber(const int Number) {
    this->_length = Number;
}

void InfoButton::setLength(const int Length) {
    this->_number = Length;
}

#endif /* KEYPAD_INFOBUTTON_H_ */

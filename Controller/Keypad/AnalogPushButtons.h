/*
 * AnalogPushButton.h
 *  Created on: Dec 30, 2017
 *  Author: Nelson Vides
 *  Forked from: https://github.com/r89m/PushButton
 */

/*
 * This needs to hold as field data:
 *      The pressed key,
 *      The length of the press.
 */

#ifndef KEYPAD_ANALOGPUSHBUTTONS_H_
#define KEYPAD_ANALOGPUSHBUTTONS_H_

#include <inttypes.h>
#include "Button.h"
#include "Bounce.h"

#define PRESSED_WHEN_HIGH B0
#define PRESSED_WHEN_LOW B1
#define ENABLE_INTERNAL_PULLUP B10

// Define button configuration callback
typedef void (*PushButtonConfigurationCallback)(Bounce&);


class AnalogPushButtons : public Button {
public:
    AnalogPushButtons(const uint8_t pin);
    virtual ~AnalogPushButtons() = default;
    const uint8_t getButtonNumber() const;

    //events
    void configureButton(PushButtonConfigurationCallback);

protected:
    bool _update_button_state();
    void _update_info();

private:
    // Data fields
    uint8_t _buttonNumber;

    // Implementation details
    Bounce _bouncer;
    const uint8_t _button_pressed_value;

    // setters
    void setButtonNumber(const uint8_t num);
};




AnalogPushButtons::AnalogPushButtons(const uint8_t pin)
    : _button_pressed_value(HIGH), _buttonNumber(0), _bouncer(Bounce(pin))
{
    pinMode(pin, INPUT);
}

void AnalogPushButtons::configureButton(PushButtonConfigurationCallback configurationCallback){
    configurationCallback(this->_bouncer);
}

bool AnalogPushButtons::_update_button_state()
{
    this->_bouncer.update();
    this->_update_info();
    if (this->_bouncer.read()) {
        Serial.println(this->_bouncer.read());
    }
    return (this->_bouncer.read() == _button_pressed_value);
}


//Thanks to https://www.youtube.com/watch?v=iXpM-v8MNAE
void AnalogPushButtons::_update_info()
{
    uint8_t num = 0;
    uint16_t val = this->_bouncer.getValue();
    if (val > 100 && val < 300) {
        setButtonNumber(4);
    } else if (val > 300 && val < 550) {
        setButtonNumber(3);
    } else if (val > 550 && val < 800) {
        setButtonNumber(2);
    } else if (val > 960) {
        setButtonNumber(1);
    }
}

const uint8_t AnalogPushButtons::getButtonNumber() const
{
    return this->_buttonNumber;
}
void AnalogPushButtons::setButtonNumber(const uint8_t num)
{
    this->_buttonNumber = num;
}


#endif /* KEYPAD_ANALOGPUSHBUTTONS_H_ */

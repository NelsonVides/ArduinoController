/*
 * PushButton.h
 *
 * Created: 18/11/2014 19:33:33
 *  Author: Richard
 */ 


#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

#include "Bounce2.h"
#include "Button.h"

namespace {
    constexpr uint8_t PRESSED_WHEN_HIGH      = 0b00000000;
    constexpr uint8_t PRESSED_WHEN_LOW       = 0b00000001;
    constexpr uint8_t ENABLE_INTERNAL_PULLUP = 0b00000010;
}

// Define button configuration callback
typedef void (*PushButtonConfigurationCallback)(Bounce&);

class PushButton : public Button {
public:
    PushButton(uint8_t pin);
    PushButton(uint8_t pit, uint8_t options);
    virtual ~PushButton() = default;

    void configureButton(PushButtonConfigurationCallback configurationCallback);

protected:
    boolean _update_button_state();

private:
    Bounce bouncer;
    uint8_t _button_pressed_value;
    void init(uint8_t pin, uint8_t options);
};

#endif /* PUSHBUTTON_H_ */

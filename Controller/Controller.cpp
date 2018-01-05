#include <Arduino.h>
#include <LiquidCrystal.h>

#include "classes/Thermometer.h"
#include "Keypad/PushButton.h"

//Thermometer
constexpr uint8_t trPin = A0;
Thermometer Therm(trPin);

//LCD
constexpr uint8_t rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal Lcd(rs, en, d4, d5, d6, d7);
//bool _stateLCD = true;

//PushButtons
PushButton button1 = PushButton(A1);
PushButton button2 = PushButton(A2);
PushButton button3 = PushButton(A3);
PushButton button4 = PushButton(A4);
void onButtonPressed(PushButton& btn);
void onButtonHeld(PushButton& btn, uint16_t duration, uint16_t repeatCount);
void onButtonReleased(PushButton& btn, uint16_t duration);

void setup() {
	Serial.begin(115200);
	Serial.println("INIT of everything");

	Lcd.begin(16, 2);
	Lcd.clear();
	Lcd.print("Arduino");
	Lcd.setCursor(2, 1);
	Lcd.print("Celcius ");
	Lcd.setCursor(10, 1);

    pinMode(8, OUTPUT);
    digitalWrite(8,HIGH);

    // When the button is first pressed, call the function onButtonPressed (further down the page)
    button1.onPress(onButtonPressed);
    button2.onPress(onButtonPressed);
    button3.onPress(onButtonPressed);
    button4.onPress(onButtonPressed);

    // Once the button has been held for 1 second (1000ms) call onButtonHeld. Call it again every 0.5s (500ms) until it is let go
    button1.onHoldRepeat(1000, 500, onButtonHeld);
    // When the button is released, call onButtonReleased
    button1.onRelease(onButtonReleased);
    button2.onRelease(onButtonReleased);
    button3.onRelease(onButtonReleased);
    button4.onRelease(onButtonReleased);
}

void loop() {
	//measure temperature //float temperatureCelsius = Therm.getCelsius();
	if (Therm.isTime()) {
        Lcd.setCursor(10, 1);
        Lcd.print(Therm.getCelsius()); //Lcd.print(temperatureCelsius);
	}

    // Update those buttons
    button1.update();
    button2.update();
    button3.update();
    button4.update();
}

#include "KeypadCallbacks.h"

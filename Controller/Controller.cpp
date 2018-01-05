#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Sim800L.h>

#include "classes/Thermometer.h"
#include "Keypad/PushButton.h"

unsigned long previousMillis = 0;   // will store last time LED was updated
constexpr uint16_t interval = 1000;     // interval at which to blink (milliseconds)
constexpr uint8_t trPin = 0;
Thermometer Therm(trPin);

constexpr uint8_t rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal Lcd(rs, en, d4, d5, d6, d7);
bool _stateLCD = true;

// Create an instance of PushButton reading digital pin 5
PushButton button1 = PushButton(A1);
PushButton button2 = PushButton(A2);
PushButton button3 = PushButton(A3);
PushButton button4 = PushButton(A4);
// btn is a reference to the button that fired the event. That means you can use the same event handler for many buttons
void onButtonPressed(PushButton& btn);
// duration reports back how long it has been since the button was originally pressed.
// repeatCount tells us how many times this function has been called by this button.
void onButtonHeld(PushButton& btn, uint16_t duration, uint16_t repeatCount);
// duration reports back the total time that the button was held down
void onButtonReleased(PushButton& btn, uint16_t duration);

//constexpr uint8_t pinPad1 = A1, pinPad2 = A2, pinPad3 = A3, pinPad4 = A4;
//Keypad pad(pinPad1, pinPad2, pinPad3, pinPad4);

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
    digitalWrite(8,_stateLCD);

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

    // Keypad state
    //pad.updateKeys();
    button1.update();
    button2.update();
    button3.update();
    button4.update();
}















// btn is a reference to the button that fired the event. That means you can use the same event handler for many buttons
void onButtonPressed(const PushButton& btn){
    int v4 = btn.getAnalogValue();
    if(btn.is(button1)){
        if (v4>100 && v4<300)
            Serial.print("Button 1");
        if (v4>300 && v4<550)
            Serial.print("Button 2");
        if (v4>550 && v4<800)
            Serial.print("Button 3");
        if (v4>960)
            Serial.print("Button 4");
    } else if (btn.is(button2)){
        if (v4>100 && v4<300)
            Serial.print("Button 5");
        if (v4>300 && v4<550)
            Serial.print("Button 6");
        if (v4>550 && v4<800)
            Serial.print("Button 7");
        if (v4>960)
            Serial.print("Button 8");
    } else if (btn.is(button3)){
        if (v4>100 && v4<300)
            Serial.print("Button 9");
        if (v4>300 && v4<550)
            Serial.print("Button 10");
        if (v4>550 && v4<800)
            Serial.print("Button 11");
        if (v4>960)
            Serial.print("Button 12");
    } else if (btn.is(button4)){
        if (v4>100 && v4<300)
            Serial.print("Button 13");
        if (v4>300 && v4<550)
            Serial.print("Button 14");
        if (v4>550 && v4<800)
            Serial.print("Button 15");
        if (v4>960)
            Serial.print("Button 16");
    } else {
       Serial.print("Hmmm, no button was");
    }
    Serial.println(" pressed");
}

// duration reports back how long it has been since the button was originally pressed.
// repeatCount tells us how many times this function has been called by this button.
void onButtonHeld(PushButton& btn, uint16_t duration, uint16_t repeatCount){
        Serial.print("button has been held for ");
        Serial.print(duration);
        Serial.print(" ms; this event has been fired ");
        Serial.print(repeatCount);
        Serial.println(" times");
}

// duration reports back the total time that the button was held down
void onButtonReleased(PushButton& btn, uint16_t duration){
        Serial.print("button released after ");
        Serial.print(duration);
        Serial.println(" ms");
}

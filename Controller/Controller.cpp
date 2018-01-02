#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Sim800L.h>

#include "classes/Thermo.h"
#include "Keypad/InfoButton.h"
#include "Keypad/Button.h"
#include "Keypad/Keypad.h"

constexpr uint8_t rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
constexpr uint8_t trPin = 0;
constexpr uint8_t btPin = A5;
constexpr uint8_t txSimPin = 11;
constexpr uint8_t rxSimPin = 12;
constexpr uint8_t rstSimPin = 13;
constexpr uint8_t pinPad1 = A1, pinPad2 = A2, pinPad3 = A3, pinPad4 = A4;

Thermo Therm(trPin);
LiquidCrystal Lcd(rs, en, d4, d5, d6, d7);
bool _stateLCD = true;
Keypad pad(pinPad1, pinPad2, pinPad3, pinPad4);
//ButtonsControls BtD(btPin);
//Sim800L Sim(rxSimPin, txSimPin, rstSimPin);

unsigned long previousMillis = 0;	// will store last time LED was updated
constexpr long interval = 1000;		// interval at which to blink (milliseconds)

//char *number;
//char *text;
//bool errorSim;
//bool sent = false;

void onButtonPressed(Button& btn){
    Serial.println(btn.getButtonNumber());
    if (btn.getButtonNumber() == 1){
        Serial.println("Button 1 pressed");
        _stateLCD = !_stateLCD;
        digitalWrite(8,_stateLCD);
    }
}

void onButtonReleased(Button& btn, uint16_t duration) {
    Serial.println("Button released");
    _stateLCD = !_stateLCD;
    digitalWrite(8,_stateLCD);
}

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
    pad.onPress(0, onButtonPressed);
    pad.onRelease(0, onButtonReleased);
    pad.onRelease(1, onButtonReleased);
    pad.onRelease(2, onButtonReleased);
    pad.onRelease(3, onButtonReleased);
}

void loop() {
	//measure temperature
	float temperatureCelsius = Therm.getCelsius();

	// Keypad state
	pad.updateKeys();

	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis;
		//Serial print
		//Serial.println(temperatureCelsius);
		//LCD
		Lcd.setCursor(10, 1);
		Lcd.print(temperatureCelsius);
	}
/*
	if (false) //(!sent)
	{
		Serial.print("Signal Quality is: ");
		Serial.println(Sim.signalQuality());

		Serial.print("Let's try sending now: ");
		number = "+48733791307";
		text = "Test hoping it works";
		errorSim = Sim.sendSms(number,text);
		sent = true;
		if (errorSim)
		{
			Serial.println("Error Sending Message");
		}
		else
		{
			Serial.println("Message Sent Successfully!");
		}
	}
	*/
}

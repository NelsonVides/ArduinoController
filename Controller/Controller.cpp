#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Sim800L.h>

#include "classes/Thermo.h"
#include "Keypad/InfoButton.h"
#include "Keypad/Button.h"
#include "Keypad/Keypad.h"

constexpr int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
constexpr int trPin = 0;
constexpr int btPin = A5;
constexpr int txSimPin = 11;
constexpr int rxSimPin = 12;
constexpr int rstSimPin = 13;
constexpr int pinPad1 = A1, pinPad2 = A2, pinPad3 = A3, pinPad4 = A4;

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
    Serial.println("Button 0 pressed");
    if (true) {                          //(btn.getButtonNumber() == 0){
        _stateLCD = !_stateLCD;
        digitalWrite(8,_stateLCD);
    } else {
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
    if (pad.onRelease(0, onButtonReleased) == CallbackAttachedResponse::attSuccessful) {
        Serial.println("Attached onReleased 0");
    }
    if (pad.onRelease(1, onButtonReleased) == CallbackAttachedResponse::attSuccessful) {
        Serial.println("Attached onReleased 1");
    }
    if (pad.onRelease(2, onButtonReleased) == CallbackAttachedResponse::attSuccessful) {
        Serial.println("Attached onReleased 2");
    }
    if (pad.onRelease(3, onButtonReleased) == CallbackAttachedResponse::attSuccessful) {
        Serial.println("Attached onReleased 3");
    }
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

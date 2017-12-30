#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Sim800L.h>

#include "classes/Thermo.h"
#include "Keypad/ButtonsControls.h"

constexpr int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
constexpr int trPin = 0;
constexpr int btPin = A5;
constexpr int txSimPin = 11;
constexpr int rxSimPin = 12;
constexpr int rstSimPin = 13;

LiquidCrystal Lcd(rs, en, d4, d5, d6, d7);
Thermo Therm(trPin);
ButtonsControls BtD(btPin);
Sim800L Sim(rxSimPin, txSimPin, rstSimPin);

unsigned long previousMillis = 0;	// will store last time LED was updated
constexpr long interval = 1000;		// interval at which to blink (milliseconds)

char *number;
char *text;
bool errorSim;
bool sent = false;

void setup() {
	Serial.begin(115200);

	Sim.begin(4800);
	Serial.println("Testing GSM SIM800L");

	Lcd.begin(16, 2);
	Lcd.clear();
	Lcd.print("Arduino");
	Lcd.setCursor(2, 1);
	Lcd.print("Celcius ");
	Lcd.setCursor(10, 1);
}

void loop() {
	//measure temperature
	float temperatureCelsius = Therm.getCelsius();

	// buttonState
	BtD.updateState();
	if (BtD.getState() == true) {
		Lcd.noDisplay();
	} else {
		Lcd.display();
	}

	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis;
		//Serial print
		Serial.println(temperatureCelsius);
		//LCD
		Lcd.setCursor(10, 1);
		Lcd.print(temperatureCelsius);
	}

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
}

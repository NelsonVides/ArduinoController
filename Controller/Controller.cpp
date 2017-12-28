#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

LiquidCrystal lcd(12, 11, 5,4,3,2);

//RF24 radio(7,8);
//const byte rxAddr[6] = "00001";

void setup() {
	lcd.begin(16,2);
	Serial.begin(115200);

//	while (!Serial);
//	Serial.begin(9600);
//	radio.begin();
//	radio.openReadingPipe(0, rxAddr);
//	radio.startListening();
}

void loop() {
	lcd.clear();
	lcd.print("Arduino");
	lcd.setCursor(2,1);
	lcd.print("Celcius ");
	lcd.setCursor(10,1);
	int sensor = analogRead(0);
	float voltage = (sensor * 500.0) / 1023.0;
	float temperatureCelsius = (voltage - 32.0)*(5.0/9.0);
	Serial.println(temperatureCelsius);
	lcd.print(temperatureCelsius);
	delay(1000);
	lcd.clear();

//	if (radio.available())
//	{
//		char text[32] = {0};
//		radio.read(&text, sizeof(text));
//		Serial.println(text);
//	}
}

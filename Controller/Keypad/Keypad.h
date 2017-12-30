/*
 * Keypad.h
 *  Created on: Dec 30, 2017
 *      Author: Nelson Vides
 */
/*
 * What do I need from this class?
 * I'll update it once every loop,
 *      Then read its values: which button, and for how long.
 *      Let's say we will return a 'PushButton' object
 *      that has type and length data fields.
 */

#ifndef CLASSES_KEYPAD_H_
#define CLASSES_KEYPAD_H_

#include "AnalogPushButton.h"

class Keypad {
public:
	Keypad(const int pin1, const int pin2, const int pin3, const int pin4);
	virtual ~Keypad();
	void updateState();
	const AnalogPushButton& getButton() const;

private:
	AnalogPushButton _button;
	const int _pin1;
	const int _pin2;
	const int _pin3;
	const int _pin4;
};



Keypad::Keypad(const int pin1, const int pin2, const int pin3, const int pin4)
	: _pin1(pin1),
	  _pin2(pin2),
	  _pin3(pin3),
	  _pin4(pin4) {}
Keypad::~Keypad() {}

/*
void set()
{
	v1 = analogRead(kb1);
	v2 = analogRead(kb2);
	v3 = analogRead(kb3);
	v4 = analogRead(kb4);

	Serial.println(v1);
//	Serial.println(v2);
//	Serial.println(v3);
//	Serial.println(v4);

	if (v1>200) {
		_state = !_state;
		Serial.println(_state);
		digitalWrite(8,_state);
	}

	delay(800);
	//Thanks to https://www.youtube.com/watch?v=iXpM-v8MNAE
	Serial.print("WCISNIETY KLAWISZ: ");


	if (v4>100 && v4<300) {
		Serial.println("1");}
	if (v4>300 && v4<550) {
		Serial.println("2");}
	if (v4>550 && v4<800) {
		Serial.println("3");}
	if (v4>960) {
		Serial.println("4");}

	if (v3>100 && v3<300){
		Serial.println("5");}
	if (v3>300 && v3<550){
		Serial.println("6");}
	if (v3>550 && v3<800){
		Serial.println("7");}
	if (v3>960){
		Serial.println("8");}

	if (v2>100 && v2<300){
		Serial.println("9");}
	if (v2>300 && v2<550){
		Serial.println("10");}
	if (v2>550 && v2<800){
		Serial.println("11");}
	if (v2>960){
		Serial.println("12");}

	if (v1>100 && v1<300){
		Serial.println("13");}
	if (v1>300 && v1<550){
		Serial.println("14");}
	if (v1>550 && v1<800){
		Serial.println("15");}
	if (v1>960){
		Serial.println("16");}
	Serial.println(" ");
	Serial.println("----------------------");
}
*/

#endif /* CLASSES_KEYPAD_H_ */

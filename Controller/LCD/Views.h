/*
 * Views.h
 *
 *  Created on: Jan 27, 2018
 *      Author: Nelson Vides
 */

#ifndef LCD_VIEWS_H_
#define LCD_VIEWS_H_

#include "LiquidCrystal_I2C.h"
#include "../AirQuality/MQ135.h"
#include "../RelayControl/Relay.h"

namespace Views {

    class theView{
    private:
        LiquidCrystal::LiquidCrystal_I2C &Lcd;
    };

    LiquidCrystal::LiquidCrystal_I2C Lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3,
            LiquidCrystal::t_backlightPol::POSITIVE);

    void ViewIntro()
    {
        pinMode(pins::lcdBckLight, OUTPUT);
        analogWrite(pins::lcdBckLight, HIGH);
        Lcd.clear();
        Lcd.print(F("Arduino Rozana"));
        Lcd.setCursor(0, 1);
        Lcd.print(F("Vides & Grochola"));
    }

    void ViewWeather()
    {
    }

    void ViewCalendar();
    void ViewSMS();
    //NOTE: Implement more Views if necessary
}

#endif /* LCD_VIEWS_H_ */

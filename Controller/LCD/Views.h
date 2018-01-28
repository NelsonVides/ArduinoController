/*
 * Views.h
 *
 *  Created on: Jan 27, 2018
 *      Author: Nelson Vides
 */

#ifndef LCD_VIEWS_H_
#define LCD_VIEWS_H_

#include "LiquidCrystal_I2C.h"

namespace Views {
    LiquidCrystal::LiquidCrystal_I2C Lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3,
            LiquidCrystal::t_backlightPol::POSITIVE);

    void ViewIntro()
    {
        Lcd.clear();
        Lcd.print(F("Arduino Rozana"));
        Lcd.setCursor(0, 1);
        Lcd.print(F("Vides & Grochola"));
    }

    void ViewWeather(float pres, float hum, float temp)
    {
        Lcd.clear();
        Lcd.print(F("Pa "));
        Lcd.print(pres, 0);
        Lcd.print(F("  RH "));
        Lcd.print(hum, 0);
        Lcd.print(F("%"));
        Lcd.setCursor(0, 1);
        Lcd.print(F("Celcius "));
        Lcd.print(temp, 2);
    }

    void ViewCalendar();
    void ViewSMS();
    //NOTE: Implement more Views if necessary
}

#endif /* LCD_VIEWS_H_ */

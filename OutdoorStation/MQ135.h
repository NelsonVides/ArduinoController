/**************************************************************************/
/*!
 @file     MQ135.h
 @author   G.Krocker (Mad Frog Labs)
 @license  GNU GPLv3

 First version of an Arduino Library for the MQ135 gas sensor
 TODO: Review the correction factor calculation. This currently relies on
 the datasheet but the information there seems to be wrong.

 @section  HISTORY

 v1.0 - First release
 */
/**************************************************************************/
#ifndef MQ135_H
#define MQ135_H
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


class MQ135 {
private:
    uint8_t _pin;

public:
    explicit MQ135(uint8_t pin);
    float getPPM();
    float getCorrectedPPM(float t, float h);
    float getRZero();
    float getCorrectedRZero(float t, float h);
private:
    inline float getCorrectionFactor(float t, float h);
    inline float getResistance();
    inline float getCorrectedResistance(float t, float h);
};
#endif

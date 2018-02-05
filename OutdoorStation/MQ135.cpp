/**************************************************************************/
/*!
 @file     MQ135.cpp
 @author   G.Krocker (Mad Frog Labs)
 @license  GNU GPLv3

 First version of an Arduino Library for the MQ135 gas sensor
 TODO: Review the correction factor calculation. This currently relies on
 the datasheet but the information there seems to be wrong.

 @section  HISTORY

 v1.0 - First release
 */
/**************************************************************************/

#include "MQ135.h"

namespace {
    /// The load resistance on the board
    constexpr float RLOAD  = 10.0;
    /// Calibration resistance at atmospheric CO2 level
    constexpr float RZERO  = 76.63;
    /// Parameters for calculating ppm of CO2 from sensor resistance
    constexpr float PARA  = 116.6020682;
    constexpr float PARB  = 2.769034857;

    /// Parameters to model temperature and humidity dependence
    constexpr float CORA  = 0.00035;
    constexpr float CORB  = 0.02718;
    constexpr float CORC  = 1.39538;
    constexpr float CORD  = 0.0018;
    constexpr float CORE  = -0.003333333;
    constexpr float CORF  = -0.001923077;
    constexpr float CORG  = 1.130128205;

    /// Atmospheric CO2 level for calibration purposes
    constexpr float ATMOCO2  = 397.13;
}


/**************************************************************************/
/*!
 @brief  Default constructor

 @param[in] pin  The analog input pin for the readout of the sensor
 */
/**************************************************************************/

MQ135::MQ135(uint8_t pin)
{
    _pin = pin;
}

/**************************************************************************/
/*!
 @brief  Get the correction factor to correct for temperature and humidity

 @param[in] t  The ambient air temperature
 @param[in] h  The relative humidity

 @return The calculated correction factor
 */
/**************************************************************************/
inline float MQ135::getCorrectionFactor(float t, float h)
{
    // Linearization of the temperature dependency curve under and above 20 degree C
    // below 20degC: fact = a * t * t - b * t - (h - 33) * d
    // above 20degC: fact = a * t + b * h + c
    // this assumes a linear dependency on humidity
    if (t < 20) {
        return CORA * t * t - CORB * t + CORC - (h - 33.) * CORD;
    } else {
        return CORE * t + CORF * h + CORG;
    }
}

/**************************************************************************/
/*!
 @brief  Get the resistance of the sensor, ie. the measurement value

 @return The sensor resistance in kOhm
 */
/**************************************************************************/
inline float MQ135::getResistance()
{
    int val = analogRead(_pin);
    return ((1023. / (float) val) - 1.) * RLOAD;
}

/**************************************************************************/
/*!
 @brief  Get the resistance of the sensor, ie. the measurement value corrected
 for temp/hum

 @param[in] t  The ambient air temperature
 @param[in] h  The relative humidity

 @return The corrected sensor resistance kOhm
 */
/**************************************************************************/
inline float MQ135::getCorrectedResistance(float t, float h)
{
    return getResistance() / getCorrectionFactor(t, h);
}

/**************************************************************************/
/*!
 @brief  Get the ppm of CO2 sensed (assuming only CO2 in the air)

 @return The ppm of CO2 in the air
 */
/**************************************************************************/
float MQ135::getPPM()
{
    return PARA * pow((getResistance() / RZERO), -PARB);
}

/**************************************************************************/
/*!
 @brief  Get the ppm of CO2 sensed (assuming only CO2 in the air), corrected
 for temp/hum

 @param[in] t  The ambient air temperature
 @param[in] h  The relative humidity

 @return The ppm of CO2 in the air
 */
/**************************************************************************/
float MQ135::getCorrectedPPM(float t, float h)
{
    return PARA * pow((getCorrectedResistance(t, h) / RZERO), -PARB);
}

/**************************************************************************/
/*!
 @brief  Get the resistance RZero of the sensor for calibration purposes

 @return The sensor resistance RZero in kOhm
 */
/**************************************************************************/
float MQ135::getRZero()
{
    return getResistance() * pow((ATMOCO2 / PARA), (1. / PARB));
}

/**************************************************************************/
/*!
 @brief  Get the corrected resistance RZero of the sensor for calibration
 purposes

 @param[in] t  The ambient air temperature
 @param[in] h  The relative humidity

 @return The corrected sensor resistance RZero in kOhm
 */
/**************************************************************************/
float MQ135::getCorrectedRZero(float t, float h)
{
    return getCorrectedResistance(t, h) * pow((ATMOCO2 / PARA), (1. / PARB));
}

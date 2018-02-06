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

/// To convert readed resistance into ohms
constexpr float RLOAD = 10.0;
/// R0 for AIR
constexpr float r0Air = 1;
/// R0 for CO **measured with 24hrs of exposure**
constexpr float r0CO = 7.41235;
/// R0 for CO2 **realized 24 hrs of exposure**
constexpr float r0CO2 = 61.10364;
/// R0 for Ethanol **measured with 24hrs of exposure**
constexpr float r0Ethanol = 26.35573;
/// R0 for Ammonium **measured with 24hrs of exposure**
constexpr float r0NH4 = 18.4409;
/// R0 for Toluene **measured with 24hrs of exposure**
constexpr float r0Toluene = 17.39102;
/// R0 for Acetone **measured with 24hrs of exposure**
constexpr float r0Acetone = 31.29273;
/// Parameters Equation for CO
constexpr float scaleFactorCO = 662.9382;
constexpr float exponentCO = 4.0241;
/// Parameters Equation for CO2
constexpr float scaleFactorCO2 = 116.6020682;
constexpr float exponentCO2 = 2.769034857;
/// Paremeters Equation for Ethanol
constexpr float scaleFactorEthanol = 75.3103;
constexpr float exponentEthanol = 3.1459;
/// Parameters Equation for NH4
constexpr float scaleFactorNH4 = 102.694;
constexpr float exponentNH4 = 2.48818;
/// Parameters Equation for Toluene
constexpr float scaleFactorToluene = 43.7748;
constexpr float exponentToluene = 3.42936;
/// Parameters Equation for Acetone
constexpr float scaleFactorAcetone = 33.1197;
constexpr float exponentAcetone = 3.36587;
/// Parameters to model temperature and humidity dependence
constexpr float CORA = 0.00035;
constexpr float CORB = 0.02718;
constexpr float CORC = 1.39538;
constexpr float CORD = 0.0018;
/// Atmospheric CO Level for calibration purposes
constexpr float atmCO = 1;
/// Atmospheric CO2 level for calibration purposes
constexpr float atmCO2 = 407.57;
/// Atmospheric Ethanol Level for calibration purposes https://www.mathesongas.com/pdfs/msds/00224106.pdf
constexpr float atmEthanol = 22.5;
/// Atmospheric NH4 level for calibration purposes
constexpr float atmNH4 = 15;
/// Atmospheric Toluene level for calibration purposes
constexpr float atmToluene = 2.9;
/// Atmospheric Acetone level for calibration purposes
constexpr float atmAcetone = 16;

class MQ135 {
private:
    const uint8_t _pin;

public:
    explicit MQ135(uint8_t pin);

    void begin();

    float getResistance() const;

    float getCOPPM() const;
    float getCO2PPM() const;
    float getEthanolPPM() const;
    float getNH4PPM() const;
    float getToluenePPM() const;
    float getAcetonePPM() const;

    float getRZeroCO() const;
    float getRZeroCO2() const;
    float getRZeroEthanol() const;
    float getRZeroNH4() const;
    float getRZeroToluene() const;
    float getRZeroAcetone() const;

    float getCO(float res) const;
    float getCO2(float res) const;
    float getEthanol(float res) const;
    float getNH4(float res) const;
    float getToluene(float res) const;
    float getAcetone(float res) const;

    float getCorrectedRZero(float r) const;
    float getCorrectedRZeroCO(float r) const;
    float getCorrectedRZeroEthanol(float r) const;
    float getCorrectedRZeroNH4(float r) const;
    float getCorrectedRZeroToluene(float r) const;
    float getCorrectedRZeroAcetone(float r) const;

    float getCorrectedResistance(float t, float h) const;
    float getCorrectionFactor(float t, float h) const;

    float getCalibratedCO2(float t, float h) const;
    float getCalibratedCO(float t, float h) const;
    float getCalibratedEthanol(float t, float h) const;
    float getCalibratedNH4(float t, float h) const;
    float getCalibratedToluene(float t, float h) const;
    float getCalibratedAcetone(float t, float h) const;

};
#endif

/******************************************************************************
MS5803_I2C.h
Library for MS5803 pressure sensor.
Bobby Schulz @ Northern Widget LLC
6/26/2014
https://github.com/sparkfun/MS5803-14BA_Breakout

The MS5803 is a media isolated temperature and pressure sensor made by
Measurment Specialties which can be used to measure either water pressure
and depth, or baramatric (atmospheric) pressure, and altitude along with that

"Instruments register only through things they're designed to register.
Space still contains infinite unknowns."
-Mr. Spock

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef TP_Downhole_h
#define TP_Downhole_h

#include <Arduino.h>
#include <MCP3421.h>
#include <MS5803.h>

//Models
enum model
{
    TP2v0 = 0x0200,
    TP2v1 = 0x0201,
    TP5v1 = 0x0501,
    TP2v2 = 0x0202,
    TP2b2 = 0x2B02,
    TP2A2 = 0x2A02
};

class TP_Downhole
{
public:
    TP_Downhole();  // Constructor
    MS5803 *pressure;
    MCP3421 *adc; //Initialize MCP3425 with default address, 0x6A
    uint8_t begin(model Model); // Collect model value

    // Return calculated temperature from sensor
    float getTemperature(uint8_t Location);
    float getTemperature();
    // Return calculated pressure from sensor
    float getPressure();

private:
    uint8_t PresMax = 2; //Maximum pressure for the sensor used
    uint8_t PresAdr = 0x77; //The address used by the MS5803 part of the TP-Downhole
    float ThermB = 3435; //Basic B value for thermistor 
    float ThermCoefs[4] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07}; //Coefficients for enhanced accuracy
    float VRef = 1.8; //Voltage referance used for thermistor
    float R0 = 10000; //Series resistor value with thermistor 
    float ThermR = 10000; //Nominal resistace value for the thermistor 
    float TempConvert(float V, float Vcc, float R, float B, float R25);
    float TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25);
};

#endif

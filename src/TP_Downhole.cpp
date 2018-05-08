/******************************************************************************
MS5803_I2C.cpp
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

#include <Wire.h> // Wire library is used for I2C
#include "TP_Downhole.h"

TP_Downhole::TP_Downhole()
// Base library type I2C
{
    pressure = new MS5803();
    adc = new MCP3421(); //Initialize MCP3425 with default address, 0x6A
}


uint8_t TP_Downhole::begin(model Model)
// Initialize library for subsequent pressure measurements
{
    switch(Model) {
        case(TP2v0):
            {
            PresMax = 2;
            PresAdr = 0x77;
            ThermB = 3435;
            VRef = 3.3;
            R0 = 47000;
            ThermR = 10000;
            float CoefTemp1[4] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp1, 4);
            }
            break;

        case(TP2v1):
            {
            PresMax = 2;
            PresAdr = 0x77;
            ThermB = 3435;
            VRef = 3.3;
            R0 = 47000;
            ThermR = 10000;
            float CoefTemp2[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp2, 4);
            }
            break;

        case(TP5v1):
            {
            PresMax = 5;
            PresAdr = 0x77;
            ThermB = 3435;
            VRef = 3.3;
            R0 = 47000;
            ThermR = 10000;
            float CoefTemp3[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp3, 4);
            }
            break;

        case(TP2v2):
            {
            PresMax = 2;
            PresAdr = 0x77;
            ThermB = 3435;
            VRef = 1.8;
            R0 = 10000;
            ThermR = 10000;
            float CoefTemp4[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp4, 4);
            }
            break;

        case(TP2b2):
            {
            PresMax = 2;
            PresAdr = 0x76;
            ThermB = 3435;
            VRef = 0; //No external thermistor 
            }
            break;

        case(TP2A2):
            {
            PresMax = 2;
            PresAdr = 0x76;
            ThermB = 3435;
            VRef = 1.8;
            R0 = 10000;
            ThermR = 10000;
            float CoefTemp6[] = {0.003354016, 0.0003074038, 1.019153E-05, 9.093712E-07};
            memcpy(ThermCoefs, CoefTemp6, 4);
            }
            break;
    }
    uint8_t ErrorPressure = pressure -> begin(PresAdr, PresMax);
    pressure -> reset();
    uint8_t ErrorADC = adc-> Begin();
    adc -> SetResolution(18);

    if(ErrorADC == 0 && ErrorPressure == 0) return 0;
    else return -1; //Retun failure is both devices are not connected 
}

float TP_Downhole::getTemperature(uint8_t Location) //Returns temp in C from either subsensor
{
    if(Location == 0) return pressure -> getTemperature(CELSIUS, ADC_512);
    if(Location == 1) {
        adc -> SetResolution(18);  //Make sure resolution is set on onboard ADC
        float Val = adc -> GetVoltage(); //Get a voltage conversion from ADC
        Val = VRef - Val; //Voltage is measure across thermistor, not relative to ground
        float Temp = TempConvert(Val, VRef, R0, ThermCoefs[0], ThermCoefs[1], ThermCoefs[2], ThermCoefs[3], ThermR);  //Make conversion to temp
        Temp = Temp - 273.15; //Convert from Kelvin to C
        return Temp;
    }
}


float TP_Downhole::getTemperature() //By default get thermistor temp value
{
    getTemperature(1); 
}


float TP_Downhole::getPressure()
// Return a pressure reading units mBar.
{
    return pressure -> getPressure(ADC_4096);
}

float TP_Downhole::TempConvert(float V, float Vcc, float R, float B, float R25){
  float Rt = (Vcc/V)*R - R;
  float T = 1/((1/B)*log(Rt/R25) + 1/298.15);
  return T;
}

float TP_Downhole::TempConvert(float V, float Vcc, float R, float A, float B, float C, float D, float R25){
  float Rt = (Vcc/V)*R - R;
  float LogRt = log(Rt/R25);
  float T = 1.0/(A + B*LogRt + C*pow(LogRt, 2.0) + D*pow(LogRt, 3.0));
  return T;
}

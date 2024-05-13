/*
********************************************************************
  Name    : CTSENS.h
  Author  : Matthew Uniac
  Date    : 23 March, 2017
  Version : 1.0
  Notes   : A library that will read a Current Transformer attached to an
  		    : analog pin and return the calculated Current value.
  Release : 
********************************************************************
*/

#include "CTSENS.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



//a function that starts the Current Transfomer for use with analogRead()
void CTSENS::begin(uint8_t pin, uint16_t numSamples, float vcc, uint16_t Te, uint8_t resistance){
_numSamples = numSamples;
_vcc = vcc;
_Te = Te;
_resistance = resistance;
_pin = pin; //sets pin number
}

//a function that starts the Current Trasformer for use with MCP3021 I2C ADC
void CTSENS::begin(MCP3021* mcp3021, uint16_t numSamples, float vcc, uint16_t Te, uint8_t resistance){
_mcp3021 = mcp3021; //pass the input to a private variable
_numSamples = numSamples;
_vcc = vcc;
_Te = Te;
_resistance = resistance;
}


//a function that reads the current transformer via an i2c ADC
float CTSENS::calcCurrentI2C(){

  float iRMS;
  int squaredAvgADC;
  double sumSamplesADC;
  float iPerStep = (_vcc / 1023) * (_Te / _resistance); // result is current(millAmps) per step of ADC

  for (uint16_t n = 0; n < _numSamples; n++) { //preform a loop while n is less than numSamples
    uint16_t valueADC = _mcp3021->readADC(); // read the adc sensor
    
    if (valueADC > 512 || valueADC < 512) {
      squaredAvgADC = (valueADC - 512); //subtract 0 point from value read
      squaredAvgADC = (squaredAvgADC * squaredAvgADC) / 2; // square the result and average to remove negatives
      sumSamplesADC = sumSamplesADC + squaredAvgADC; //add squaredAvgADC to total
    }
  }
  //Serial.println(iPerStep);
  iRMS = iPerStep * sqrt(sumSamplesADC / _numSamples); // 
  iRMS = iRMS * sqrt(2);
  //iRMS = iRMS - baseline;

  sumSamplesADC = 0; //reset the count

  return iRMS;
}



//a function that calculates CurrentRMS and returns it's value
float CTSENS::calcCurrent()
{

  float iRMS;
  int squaredAvgADC;
  double sumSamplesADC;
  float iPerStep = (_vcc / 1023) * (_Te / _resistance); // result is current(millAmps) per step of ADC

  for (uint16_t n = 0; n < _numSamples; n++) { //preform a loop while n is less than numSamples
    uint16_t valueADC = analogRead(_pin); // read the adc sensor
    
    if (valueADC > 512 || valueADC < 512) {
      squaredAvgADC = (valueADC - 512); //subtract 0 point from value read
      squaredAvgADC = (squaredAvgADC * squaredAvgADC) / 2; // square the result and average to remove negatives
      sumSamplesADC = sumSamplesADC + squaredAvgADC; //add squaredAvgADC to total
    }
  }
  //Serial.println(iPerStep);
  iRMS = iPerStep * sqrt(sumSamplesADC / _numSamples); // 
  iRMS = iRMS * sqrt(2);
  //iRMS = iRMS - baseline;

  sumSamplesADC = 0; //reset the count

  return iRMS;
}

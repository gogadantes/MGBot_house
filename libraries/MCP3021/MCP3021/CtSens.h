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

#ifndef CTSENS_h
#define CTSENS_h



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "mcp3021.h"

class CTSENS {
	public:
// 	CTSENS::CTSENS(MCP3021 * mcp3021);
 
    void begin(uint8_t pin, uint16_t numSamples, float vcc, uint16_t Te, uint8_t resistance); //function to setup CT sensor reading using analogREAD
	void begin(MCP3021* mcp3021, uint16_t numSamples, float vcc, uint16_t Te, uint8_t resistance); //function to setup CT sensor reading using I2C ADC
	
	float calcCurrentI2C();
	float calcCurrent(); //function that returns calculated current	
    
	private:	
  		
  		//global
  		uint16_t _numSamples;
		float _vcc;
		uint16_t _Te;
		uint8_t _resistance;
		
		//used in onboard setup
		uint8_t _pin;

		//used in I2C setup
		MCP3021* _mcp3021;

		  


};

#endif

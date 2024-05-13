/*
 * TLC59108: Arduino library to control TI TLC59108/TLC59108F/TLC59208 LED drivers
 * 
 * (C) 2013 Christopher Smith <chrylis@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

extern "C"
{
#include <inttypes.h>
}

#include "TLC59108.h"

TLC59108::TLC59108(const byte i2c_address): addr(i2c_address)
{
}

uint8_t TLC59108::setRegister(const uint8_t reg, const uint8_t value)
{
   Wire.beginTransmission(addr);
   Wire.write(reg);
   Wire.write(value);
   return Wire.endTransmission();
}

uint8_t TLC59108::setRegisters(const uint8_t startReg, const uint8_t values[], const uint8_t numValues)
{
	Wire.beginTransmission(addr);
	Wire.write(startReg | AUTO_INCREMENT::ALL);
	for(uint8_t i = 0; i < numValues; i++)
		Wire.write(values[i]);
	return Wire.endTransmission();
}

int TLC59108::readRegister(const uint8_t reg) const
{
   Wire.beginTransmission(addr);
   Wire.write(reg);
   if(!Wire.endTransmission())
     return -1;

   Wire.requestFrom(addr, (uint8_t) 1);
   if(Wire.available())
     return Wire.read();
   else
     return -1;
}

uint8_t TLC59108::readRegisters(uint8_t *dest, const uint8_t startReg, const uint8_t num) const {
	Serial.println("in readRegisters");
	Wire.beginTransmission(addr);
	Wire.write(startReg | AUTO_INCREMENT::ALL);
	if(Wire.endTransmission())
		return 0;

	uint8_t bytesRead = 0;
	Wire.requestFrom(addr, num);
	while(Wire.available() && (bytesRead < num)) {
		(*dest) = (uint8_t) Wire.read();
		dest++;
		bytesRead++;
	}

	return bytesRead;
}

bool TLC59108::getAllBrightness(uint8_t dutyCycles[]) const {
	return (readRegisters(dutyCycles, REGISTER::PWM0::ADDR, NUM_CHANNELS) == NUM_CHANNELS);
}

uint8_t TLC59108::init(const uint8_t hwResetPin)
{
   if(hwResetPin)
     {
	pinMode(hwResetPin, OUTPUT);
	digitalWrite(hwResetPin, LOW);
	delay(1);
	digitalWrite(hwResetPin, HIGH);
	delay(1);
     }

   return setRegister(REGISTER::MODE1::ADDR, REGISTER::MODE1::ALLCALL);
}

uint8_t TLC59108::setBrightness(const uint8_t pwmChannel, const uint8_t dutyCycle)
{
   if(pwmChannel > 7)
     return ERROR::EINVA;

   return setRegister(pwmChannel + 2, dutyCycle);
}

uint8_t TLC59108::setLedOutputMode(const uint8_t outputMode)
{
   if(outputMode & 0xfc)
     return ERROR::EINVA;

   byte regValue = (outputMode << 6) | (outputMode << 4) | (outputMode << 2) | outputMode;

   uint8_t retVal = setRegister(REGISTER::LEDOUT0::ADDR, regValue);
   retVal &= setRegister(REGISTER::LEDOUT1::ADDR, regValue);
   return retVal;
}

uint8_t TLC59108::setAllBrightness(const uint8_t dutyCycle)
{
   Wire.beginTransmission(addr);
   Wire.write(REGISTER::PWM0::ADDR | AUTO_INCREMENT::IND);
   for(uint8_t i=0; i<NUM_CHANNELS; i++)
     Wire.write(dutyCycle);
   return Wire.endTransmission();
}

uint8_t TLC59108::setAllBrightness(const uint8_t dutyCycles[]) 
{
	return setRegisters(REGISTER::PWM0::ADDR, dutyCycles, NUM_CHANNELS);
}

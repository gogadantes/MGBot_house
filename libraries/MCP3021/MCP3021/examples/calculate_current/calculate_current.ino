#include <Wire.h>
#include "mcp3021.h"
#include "CtSens.h"

//define GPIO pins
const uint8_t sda = A4; //define i2c SDA pin as A4
const uint8_t scl = A5; //define i2c SCL pin as A5

//declare varibles
uint16_t numSamples = 500; //number of samples the sensor should take each loop
float vcc = 5.0; //sensor vcc value
uint16_t Te = 1021; //winding number
uint8_t resistor = 27; //resistor used
float iRMS; // varible to return CT current caluclation.

// create instances of library objects
MCP3021 mcp3021; // create an instance of mcp3021
CTSENS ct; // create an instance of CtSens


void setup() {
  //setup i2c pins
  pinMode (scl, INPUT_PULLUP);
  pinMode (sda, INPUT_PULLUP);

  //enable serial communication
  Serial.begin(115200);

  //setup i2c communication
  Wire.begin();

  //begin communication with mcp3021-default deviceId is 5
  mcp3021.begin();

  //setup the ct sensor
  ct.begin(&mcp3021, numSamples, vcc, Te, resistor); //setup for I2C ADC reading

}
void loop() {
  iRMS = ct.calcCurrentI2C(); // calculate current CtSens library using i2c "analogRead"
  Serial.print(F("Offboard ADC: "));
  Serial.println(iRMS); //debug output
}


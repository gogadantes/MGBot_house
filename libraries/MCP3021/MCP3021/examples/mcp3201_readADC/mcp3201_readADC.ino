#include <Wire.h>

#include "mcp3021.h"

//define GPIO pins
const uint8_t sda = A4; //define i2c SDA pin as A4
const uint8_t scl = A5; //define i2c SCL pin as A5
//variables for ADC
uint8_t adcDeviceId =  0b00000101; //the deviceID
MCP3021 mcp3021;

void setup() {
  // put your setup code here, to run once:
  //setup i2c pins
  pinMode (scl, INPUT_PULLUP);
  pinMode (sda, INPUT_PULLUP);

  //enable serial communication
  Serial.begin(115200);

  //setup i2c communication
  Wire.begin();

  //begin communication with mcp3021
  mcp3021.begin(adcDeviceId);

}

void loop() {
  // put your main code here, to run repeatedly:
      //testing i2c adc
    uint16_t valueADC = mcp3021.readADC();
    Serial.println(valueADC);
}

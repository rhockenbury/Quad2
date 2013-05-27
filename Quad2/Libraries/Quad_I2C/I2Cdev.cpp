/*
 * I2Cdev.cpp
 *
 *  Created on: April 21, 2013
 *      Author: Ryler Hockenbury
 *
 * Library for I2C bus device interaction
 * WARNING:  Must include Wire.h in main sketch
 *
 */

#include "I2Cdev.h"

I2Cdev::I2Cdev() { /* empty */ }

uint16_t I2Cdev::readTimeout = 1000;

//Method for writing a bit to an address on an I2C device
bool I2Cdev::writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, 
  uint8_t data)
{
  uint8_t b; 
  readByte(devAddr, regAddr, &b); 
  b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
  return writeByte(devAddr, regAddr, b);
}



//Method for writing bits to an address on an I2C device
bool I2Cdev::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, 
  uint8_t length, uint8_t data)
{
  uint8_t b;
    if (readByte(devAddr, regAddr, &b) != 0) {
      uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
      data <<= (bitStart - length + 1);   // shift data into correct position
      data &= mask;   // zero all non-important bits in data
      b &= ~(mask);   // zero all important bits in existing byte
      b |= data;      // combine data with existing byte
      return writeByte(devAddr, regAddr, b);
    } 
    else 
      return false;

}



//Method for writing a byte to an address on an I2C device
bool I2Cdev::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) 
{
  return writeBytes(devAddr, regAddr, 1, &data);  
}



//Method for writing bytes to an address on an I2C device
bool I2Cdev::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, 
   uint8_t *data)
{
  #ifdef I2CDEV_SERIAL_DEBUG
    Serial.print("I2C (0x"); 
    Serial.print(devAddr, HEX); 
    Serial.print(") writing "); 
    Serial.print(length, DEC); 
    Serial.print(" byte(s)to 0x"); 
    Serial.print(regAddr, HEX); 
    Serial.print("..."); 
  #endif

  uint8_t status = 0;

  Wire.beginTransmission(devAddr); 
  Wire.write(regAddr); 

  uint8_t i;
  for(i = 0; i < length; i++)
  { 
  	Wire.write( (uint8_t) data[i]);

    #ifdef I2CDEV_SERIAL_DEBUG
    Serial.print(data[i], HEX);
    if (i + 1 < length) Serial.print("  ");
    #endif
  }

  status = Wire.endTransmission();

  #ifdef I2CDEV_SERIAL_DEBUG
       Serial.println(". Done.");
  #endif

  return (status == 0);
}



//Method for reading a bit from an address on an I2C device
int8_t I2Cdev::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, 
    uint8_t *data, uint16_t TIMEOUT)
{
  uint8_t b;
  uint8_t count = readByte(devAddr, regAddr, &b, TIMEOUT); 

  *data = b & (0x1 << bitNum); 
  return count; 
}



//Method for reading bits from an address on an I2C device
int8_t I2Cdev::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, 
   uint8_t length, uint8_t *data, uint16_t timeout) 
{
  uint8_t count, b;
  if ((count = readByte(devAddr, regAddr, &b, timeout)) != 0) 
  {
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    b &= mask;
    b >>= (bitStart - length + 1);
    *data = b;
  }

  return count;
}



//Method for reading a byte from an address on an I2C device
int8_t I2Cdev::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, 
  uint16_t TIMEOUT)
{
  return readBytes(devAddr, regAddr, 1, data, TIMEOUT); 
}



//Method for reading bytes from an address on an I2C device
int8_t I2Cdev::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, 
  uint8_t *data, uint16_t TIMEOUT) 
{
  #ifdef I2CDEV_SERIAL_DEBUG
    Serial.print("I2C (0x"); 
    Serial.print(devAddr, HEX); 
    Serial.print(") reading "); 
    Serial.print(length, DEC); 
    Serial.print(" byte(s) from 0x"); 
    Serial.print(regAddr, HEX); 
    Serial.print("..."); 
  #endif

  uint32_t tStart = millis(); 
  uint8_t count = 0; // byte transfer counter 

  Wire.beginTransmission(devAddr);
  Wire.write(regAddr);
  Wire.endTransmission();

  Wire.beginTransmission(devAddr); 
  Wire.requestFrom(devAddr, length);

  while( Wire.available() && (TIMEOUT == 0 || (millis() - tStart) < TIMEOUT) )
  { 
    data[count] = Wire.read(); 

    #ifdef I2CDEV_SERIAL_DEBUG
      Serial.print(data[count], HEX);
      if (count + 1 < length) Serial.print(" ");
    #endif

    count++;
  }

  Wire.endTransmission(); 

  //check for timeout
  if( TIMEOUT > 0 && (millis() - tStart) >=TIMEOUT && count < length )
    count = -1; 

  #ifdef I2CDEV_SERIAL_DEBUG
    Serial.print(". Done ("); 
    Serial.print(count, DEC); 
    Serial.print(" read)."); 
  #endif

  return count; 
}



/*
 * storage.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: Ryler Hockenbury
 *
 *  Read and write from EEPROM
 */


void readEEPROM() {

}

void writeEEPROM() {

}


float readFloat(int address) {
  union floatStore {
    byte floatByte[4];
    unsigned short floatUShort[2];
    float floatVal;
  } floatOut;


  for (int i = 0; i < 4; i++) {
    floatOut.floatByte[i] = EEPROM.read(address + i);
  }

  return floatOut.floatVal;
}

void writeFloat(float value, int address) {
  union floatStore {
    byte floatByte[4];
    unsigned short floatUShort[2];
    float floatVal;
  } floatIn;

  floatIn.floatVal = value;

  for (int i = 0; i < 4; i++) {
    EEPROM.write(address + i, floatIn.floatByte[i]);
  }
}

void updateFloat(float value, int address) {

}


  long readLong(int address) {
    union longStore {
      byte longByte[4];
      unsigned short longUShort[2];
      long longVal;
    } longOut;

    for (byte i = 0; i < 4; i++) {
      longOut.longByte[i] = EEPROM.read(address + i);
    }

    return longOut.longVal;
  }

  void writeLong(long value, int address) {
    union longStore {
      byte longByte[4];
      unsigned short longUShort[2];
      long longVal;
    } longIn;

    longIn.longVal = value;

    for (int i = 0; i < 4; i++) {
      EEPROM.write(address + i, longIn.longByte[i]);
    }

  }

  void updateLong(long vaue, int address) {

  }







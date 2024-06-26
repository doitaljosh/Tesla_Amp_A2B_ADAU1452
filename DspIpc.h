#ifndef __DSPIPC_H__
#define __DSPIPC_H__

#include <Arduino.h>

// Include the SPI library if SPI is enabled, otherwise include the I2C library.
#include <SPI.h>
#include <Wire.h>

// Constants
#define FIXPOINT_T 0
#define INTEGER_T 1

typedef unsigned short dspdata_t;
typedef unsigned short dspreg_t;

const int sampleRate = 48000;

// Function prototypes
int dspWriteReg_Block(int addr, int len, byte data[]);
int dspWriteReg_Int(int addr, int32_t data);
int dspWriteReg_Float(int addr, double data);
int dspDelay(byte dspAddr, int len, byte data[]);
int dspReadReg_Byte(int addr, int len, byte *data);
int32_t dspReadReg_Int(int addr, int len);
double dspReadReg_Float(int addr);
void printDspReg(int addr, int len);
int dspSetLevel(int level, int numLevels, int _addr, byte data[][4]);
int dspSafeload(int addr, int numSafeload, byte data[5]);

#endif
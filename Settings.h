#ifndef __SETTINGS___
#define __SETTINGS___

#include <SPI.h>

// IO Pins
int DSP_PIN_SS = 38;
int DSP_PIN_RST = 5;

// I2C Addresses
#define I2CADDR_DSP 0x38
#define I2CADDR_ADAU1761 0x39
#define I2CADDR_EEPROM_AMP 0x56
#define I2CADDR_EEPROM_DSP 0x50
#define I2CADDR_A2B_MASTER 0x68
#define I2CADDR_A2B_SLAVE 0x69
#define I2CADDR_FDA2100 0x6e
#define I2CADDR_TDA7802 0x6c
#define I2CADDR_PCA9538 0x70

// I2C Settings
const int I2C_MAX_DATA_LEN = 30;


// SPI Settings
// SPISettings spiSettings(20000000, LSBFIRST, SPI_MODE0);

// A2B Settings
const int masterNodeID = 0;
const int ampNodeID = 1;
const int FOHCMicNodeID = -1;
// int FOHCMicNodeID = 1;
// int ampNodeID = 2;

const int audioBitRate = 32;

unsigned char masterNodeChipID[4] = {0xAD, 0x02, 0x25, 0x01};
unsigned char a2bAmpExpectedChipID[4] = {0xAD, 0x10, 0x21, 0x01};
unsigned char FOHCMicExpectedChipID[4] = {0xAD, 0x10, 0x21, 0x01};

#endif
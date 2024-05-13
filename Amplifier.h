#ifndef __PCA9538__
#define __PCA9538__

#include "A2B.h"

#define PCA9538_MAX_GPIOS 8

#define PCA9538_IN_PORT 0x00
#define PCA9538_OUT_PORT 0x01
#define PCA9538_INV 0x02
#define PCA9538_CFG 0x03

#define PIN_FDA2100_HWMUTE 0
#define PIN_TDA7802_EN 1
#define PIN_TDA7802_HWMUTE_N 2
#define PIN_BOOST_OVERTEMP 3
#define PIN_AMP_STATUS_LED 4
#define PIN_FDA2100_EN 5
#define PIN_AMPS_OVERTEMP 7

#define OUTPUT 0
#define LOW 0
#define INPUT 1
#define HIGH 1

int pcaWritePin(int gpio, bool state);
bool pcaReadPin(int gpio);
int pcaWriteReg(byte reg, byte mask);
byte pcaReadReg(byte reg);
int pcaSetPinDirection(int gpio, bool direction);
int pcaInvertPinPolarity(int gpio, bool polarity);
char* getAmplifierId(void);
int amplifierInit(void);

static unsigned int amplifierLocalA2bConfig [21][2] = {
  {AD242x_REG_BCDNSLOTS, 0},
  {AD242x_REG_LDNSLOTS, 8}, // 8 local TDM downstream slots for amp
  {AD242x_REG_LUPSLOTS, 0},
  {AD242x_REG_DNSLOTS, 0},
  {AD242x_REG_UPSLOTS, 0},
  {AD242x_REG_I2CCFG, 0x00},
  {AD242x_REG_I2SGCFG, A2B_I2SGCFG_TDMMODE_TDM4}, // TDM4 mode
  {AD242x_REG_I2SCFG, 0x03}, // TX0EN, TX1EN: 8 channels split across two TDM4 outputs
  {AD242x_REG_I2SRATE, 0x00},
  {AD242x_REG_SYNCOFFSET, 0x00},
  {AD242x_REG_PDMCTL, 0x10}, // Enable high-pass filter
  {AD242x_REG_ERRMGMT, 0x00},
  {0x49, 0x00}, // Undocumented register
  {AD242x_REG_GPIODAT, 0x00},
  {AD242x_REG_GPIOOEN, 0x00},
  {AD242x_REG_GPIOIEN, 0x00},
  {AD242x_REG_PINTEN, 0x00},
  {AD242x_REG_PINTINV, 0x00},
  {AD242x_REG_PINCFG, 0x01}, // High drive strength for pins
  {AD242x_REG_INTMSK, 0x00},
  {(AD242x_REG_INTMSK + 1), 0x00}
};

unsigned int tda7802ConfigData[6] = {
    0x0f, 0x00, 0x18, 0x49, 0x00, 0x01
};

unsigned int fda2100ConfigData[15] = {
    0x5c, 0x07, 0x0f, 0x00,
    0x00, 0x11, 0x11, 0x11, 
    0x11, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x03
};

#endif
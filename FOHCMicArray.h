#ifndef __FOHCMICARRAY__
#define __FOHCMICARRAY__

#include "A2B.h"

static unsigned int fohcMicLocalA2bConfig[21][2] {
  {AD242x_REG_BCDNSLOTS, 0},
  {AD242x_REG_LDNSLOTS, 0},
  {AD242x_REG_LUPSLOTS, 3}, // 3 local upstream slots from mic
  {AD242x_REG_DNSLOTS, 8}, // 8 downstream slots to amplifier
  {AD242x_REG_UPSLOTS, 0},
  {AD242x_REG_I2CCFG, 0x00},
  {AD242x_REG_I2SGCFG, A2B_I2SGCFG_TDMMODE_TDM2}, // TDM2 mode
  {AD242x_REG_I2SCFG, 0x30}, // RX0EN, RX1EN: 3 channels split across two TDM2 inputs
  {AD242x_REG_I2SRATE, 0x00},
  {AD242x_REG_SYNCOFFSET, 0x00},
  {AD242x_REG_PDMCTL, 0x17}, // Enable high-pass filter, PDM0 enabled with stereo, PDM1 enabled with mono
  {AD242x_REG_ERRMGMT, 0x00},
  {0x49, 0x00}, // Undocumented register
  {AD242x_REG_GPIODAT, 0x00},
  {AD242x_REG_GPIOOEN, 0x00},
  {AD242x_REG_GPIOIEN, 0x00},
  {AD242x_REG_PINTEN, 0x00},
  {AD242x_REG_PINTINV, 0x00},
  {AD242x_REG_PINCFG, 0x01}, // High drive strength for pins
  {AD242x_REG_INTMSK, 0x10}, // Enable switch reporting error interrupt
  {(AD242x_REG_INTMSK + 1), 0x00}
}

#endif
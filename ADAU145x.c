#include <Arduino.h>
#include "ADAU145x.h"
#include "DspIpc.h"
#include "Firmware.h"
#include "Settings.h"

void dspSoftReset(bool state) {
  byte data[2];
  data = {0x00, state};
  dspWriteReg_Block(SOFT_RESET, 2, data);
}

void dspHibernate(bool state) {
  byte data[2];
  data = {0x00, state};
  dspWriteReg_Block(HIBERNATE, 2, data);
}

void dspStartCore(bool state) {
  byte data[2];
  data = {0x00, state};
  dspWriteReg_Block(START_CORE, 2, data);
}

void dspKillCore(bool state) {
  byte data[2];
  data = {0x00, state};
  dspWriteReg_Block(KILL_CORE, 2, data);
}

int dspUploadProgramData(void) {
  return dspWriteReg_Block(PROGRAM_ADDR_IC_1, PROGRAM_SIZE_IC_1, Program_Data_IC_1);
}

int dspUploadDm0(void) {
  return dspWriteReg_Block(PARAM_ADDR_IC_1, PARAM_SIZE_IC_1, Param_Data_IC_1);
}

int dspUploadDm1(void) {
  return dspWriteReg_Block(DM1_DATA_ADDR_IC_1, DM1_DATA_SIZE_IC_1, DM1_DATA_Data_IC_1);
}

int dspInit(void) {
  int ret = 0;

  byte regData[2];

  // DSP power-up sequence
  dspSoftReset(0);
  dspSoftReset(1);

  delayMicroseconds(1);

  dspHibernate(0);
  dspHibernate(1);

  delayMicroseconds(1);

  dspKillCore(1);

  // Setup PLL
  regData = {0x00, 0x60};
  dspWriteReg_Block(PLL_CTRL0, 2, regData);
  regData = {0x00, 0x02};
  dspWriteReg_Block(PLL_CTRL0, 2, regData);
  regData = {0x00, 0x05};
  dspWriteReg_Block(MCLK_OUT, 2, regData);
  regData = {0x00, 0x01};
  dspWriteReg_Block(PLL_ENABLE, 2, regData);

  delayMicroseconds(10666);

  // Configure power domains
  regData = {0x4F, 0xFF};
  dspWriteReg_Block(POWER_ENABLE0, 2, regData);
  regData = {0x00, 0x00};
  dspWriteReg_Block(POWER_ENABLE1, 2, regData);

  // Upload code into DSP memory
  ret += dspUploadProgramData();
  ret += dspUploadDm0();
  ret += dspUploadDm1();

  // Set the DSP program start address
  dspWriteReg_Block(START_ADDRESS, 2, DSP_START_ADDR);
  regData = {0x00, 0x02};
  dspWriteReg_Block(START_PULSE, 2, regData);

  // Configure other registers

  // Run the DSP core
  dspKillCore(0);
  dspStartCore(0);
  dspStartCore(1);

  dspInitDone = true;

  return STATE_DSP_INIT_DONE;
}
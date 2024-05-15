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
  if ((PROGRAM_SIZE_IC_1 <= PROGMEM_MAX_SIZE) && (sizeof(Program_Data_IC_1) <= PROGMEM_MAX_SIZE)) {
    dspWriteReg_Block(PROGRAM_ADDR_IC_1, PROGRAM_SIZE_IC_1, Program_Data_IC_1);

    return 1
  } else {
    Serial.println("E: DSP Program data size too large.");
    Serial.print(PROGMEM_MAX_SIZE);
    Serial.print(", actual size: ");
    Serial.print(sizeof(Program_Data_IC_1));
    Serial.println();

    return -1
  }
}

int dspUploadDm0(void) {
  if ((PARAM_SIZE_IC_1 <= DM0_MAX_SIZE) && (sizeof(Param_Data_IC_1) <= DM0_MAX_SIZE)) {
    dspWriteReg_Block(PARAM_ADDR_IC_1, PARAM_SIZE_IC_1, Param_Data_IC_1);

    return 1
  } else {
    Serial.print("E: DSP Parameter data size too large. Maximum size: ");
    Serial.print(DM0_MAX_SIZE);
    Serial.print(", actual size: ");
    Serial.print(sizeof(Param_Data_IC_1));
    Serial.println();

    return -1
  }
}

int dspUploadDm1(void) {
  if ((DM1_DATA_SIZE_IC_1 <= DM1_MAX_SIZE) && (sizeof(DM1_DATA_Data_IC_1) <= DM1_MAX_SIZE)) {
    dspWriteReg_Block(DM1_DATA_ADDR_IC_1, DM1_DATA_SIZE_IC_1, DM1_DATA_Data_IC_1);
  
    return 1;
  } else {
    Serial.print("E: DSP DM1 data size too large. Maximum size: ");
    Serial.print(DM1_MAX_SIZE);
    Serial.print(", actual size: ");
    Serial.print(sizeof(DM1_DATA_Data_IC_1));
    Serial.println();

    return -1
  }
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

  if (ret == 3) {
    return STATE_DSP_INIT_DONE;
  else {
    return STATE_DSP_NEEDS_REINIT;
  }
}
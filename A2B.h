#ifndef __A2B_H__
#define __A2B_H__

#include <Arduino.h>
#include <Wire.h>
#include "Settings.h"
#include "Utils.h"

#define AD242x_REG_CHIP 0x00
#define AD242x_REG_NODEADR 0x01
#define AD242x_REG_VENDOR 0x02
#define AD242x_REG_PRODUCT 0x03
#define AD242x_REG_VERSION 0x04
#define AD242x_REG_CAPABILITY 0x05
#define AD242x_REG_SWCTL 0x09
#define AD242x_REG_BCDNSLOTS 0x0a
#define AD242x_REG_LDNSLOTS 0x0b
#define AD242x_REG_LUPSLOTS 0x0c
#define AD242x_REG_DNSLOTS 0x0d
#define AD242x_REG_UPSLOTS 0x0e
#define AD242x_REG_RESPCYCS 0x0f
#define AD242x_REG_SLOTFMT 0x10
#define AD242x_REG_DATCTL 0x11
#define AD242x_REG_CONTROL 0x12
#define AD242x_REG_DISCVRY 0x13
#define AD242x_REG_SWSTAT 0x14
#define AD242x_REG_INTSTAT 0x15
#define AD242x_REG_INTSRC 0x16
#define AD242x_REG_INTTYPE 0x17
#define AD242x_REG_INTPND 0x18
#define AD242x_REG_INTMSK 0x1b
#define AD242x_REG_BECCTL 0x1e
#define AD242x_REG_BECNT 0x1f
#define AD242x_REG_TESTMODE 0x20
#define AD242x_REG_ERRCNT 0x21
#define AD242x_REG_NODE 0x29
#define AD242x_REG_DISCSTAT 0x2b
#define AD242x_REG_TXACTL 0x2e
#define AD242x_REG_TXBCTL 0x30
#define AD242x_REG_LINTTYPE 0x3e
#define AD242x_REG_I2CCFG 0x3f
#define AD242x_REG_PLLCTL 0x40
#define AD242x_REG_I2SGCFG 0x41
#define AD242x_REG_I2SCFG 0x42
#define AD242x_REG_I2SRATE 0x43
#define AD242x_REG_I2STXOFFSET 0x44
#define AD242x_REG_I2SRXOFFSET 0x45
#define AD242x_REG_SYNCOFFSET 0x46
#define AD242x_REG_PDMCTL 0x47
#define AD242x_REG_ERRMGMT 0x48
#define AD242x_REG_GPIODAT 0x4a
#define AD242x_REG_GPIODATSET 0x4b
#define AD242x_REG_GPIODATCLR 0x4c
#define AD242x_REG_GPIOOEN 0x4d
#define AD242x_REG_GPIOIEN 0x4e
#define AD242x_REG_GPIOIN 0x4f
#define AD242x_REG_PINTEN 0x50
#define AD242x_REG_PINTINV 0x51
#define AD242x_REG_PINCFG 0x52
#define AD242x_REG_I2STEST 0x53
#define AD242x_REG_RAISE 0x54
#define AD242x_REG_GENERR 0x55
#define AD242x_REG_I2SRRATE 0x56
#define AD242x_REG_I2SRRCTL 0x57
#define AD242x_REG_I2SRRSOFFS 0x58
#define AD242x_REG_CLK1CFG 0x59
#define AD242x_REG_CLK2CFG 0x5a
#define AD242x_REG_BMMCFG 0x5b
#define AD242x_REG_SUSCFG 0x5c
#define AD242x_REG_PDMCTL2 0x5d
#define AD242x_REG_UPMASK 0x60
#define AD242x_REG_UPOFFSET 0x64
#define AD242x_REG_DNMASK 0x65
#define AD242x_REG_DNOFFSET 0x69
#define AD242x_REG_CHIPID 0x6a
#define AD242x_REG_GPIODEN 0x80
#define AD242x_REG_GPIODMSK 0x81
#define AD242x_REG_GPIODDAT 0x89
#define AD242x_REG_GPIODINV 0x8a
#define AD242x_REG_MBOX0CTL 0x90
#define AD242x_REG_MBOX0STAT 0x91
#define AD242x_REG_MBOX0DAT 0x92
#define AD242x_REG_MBOX1CTL 0x96
#define AD242x_REG_MBOX1STAT 0x97
#define AD242x_REG_MBOX1DAT 0x98

#define MBOX_DAT_SIZE 4

#define A2B_NODEADR_BCAST_DIS 0
#define A2B_NODEADR_BCAST_ENA 1
#define A2B_NODEADR_PERI_DIS 0
#define A2B_NODEADR_PERI_ENA 1

#define A2B_INTSRC_MASTER 0
#define A2B_INTSRC_SUB 1

#define A2B_SWCTL_MODE_USE_VSSNINT_SWPEXT 0x00
#define A2B_SWCTL_MODE_DOWNSTRM_SELFPWR_UNTERM 0x10
#define A2B_SWCTL_MODE_USE_VIN 0x20
#define A2B_SWCTL_ENSW_DISABLED 0x00
#define A2B_SWCTL_ENSW_ENABLED 0x01

#define A2B_I2SGCFG_INVERT_SYNC (1 << 7)
#define A2B_I2SGCFG_EARLY_SYNC (1 << 6)
#define A2B_I2SGCFG_ALT_SYNC (1 << 5)
#define A2B_I2SGCFG_TDM_SIZE_32BIT (0 << 4)
#define A2B_I2SGCFG_TDM_SIZE_16BIT (1 << 4)
#define A2B_I2SGCFG_RXONDTX1 (1 << 3)
#define A2B_I2SGCFG_TDMMODE_TDM2 0x00
#define A2B_I2SGCFG_TDMMODE_TDM4 0x01
#define A2B_I2SGCFG_TDMMODE_TDM8 0x02
#define A2B_I2SGCFG_TDMMODE_TDM16 0x04

#define A2B_I2SCFG_RXBCLK_INVERT (1 << 7)
#define A2B_I2SCFG_RX2PIN (1 << 6)
#define A2B_I2SCFG_RX1EN (1 << 5)
#define A2B_I2SCFG_RX0EN (1 << 4)
#define A2B_I2SCFG_TXBCLK_INVERT (1 << 3)
#define A2B_I2SCFG_TX2PIN (1 << 2)
#define A2B_I2SCFG_TX1EN (1 << 1)
#define A2B_I2SCFG_TX0EN 1

typedef struct {
  bool MstrSub;
  int nodeId;
  int intType;
} a2bInt_t;

enum intTypes {
  HDCNTERR = 0,
  DATADECERR = 1,
  CRCERR = 2,
  DATAPARITYERR = 3,
  BECOVERFLOW = 4,
  SRFMISSERR = 5,
  SRFCRCERR = 6,
  SHORT_BUSPOS2GND = 9,
  SHORT_BUSNEG2VBAT = 10,
  SHORT_BUSPOS2BUSNEG = 11,
  CABLE_DISCONNECTED = 12,
  CABLE_REVERSED = 13,
  UNDETERMINED = 15,
  IO0PND = 16,
  IO1PND = 17,
  IO2PND = 18,
  IO3PND = 19,
  IO4PND = 20,
  IO5PND = 21,
  IO6PND = 22,
  IO7PND = 23,
  DISCOVERY_DONE = 24,
  I2CERR = 25,
  INTCRCERR = 26,
  SHORT_REMOTE_BUSNEG2GND = 41,
  SHORT_REMOTE_BUSPOS2VBAT = 42,
  MBOX0_FULL = 48,
  MBOX0_EMPTY = 49,
  MBOX1_FULL = 50,
  MBOX1_EMPTY = 51,
  INTMSGERR = 128,
  STARTUPERR = 252,
  SUB_INTTYPE_READERR = 253,
  STBY_DONE = 254,
  MSTR_RUNNING = 255
};

mappedMsg_t lineFaultStrings[] = {
  {SRFMISSERR, "SRF Miss Error"},
  {BECOVERFLOW, "Bit Error Count Overflow"},
  {UNDETERMINED, "Indeterminate Fault"},
  {SHORT_REMOTE_BUSNEG2GND, "Negative Wire Shorted to GND"},
  {SHORT_REMOTE_BUSPOS2VBAT, "Positive Wire Shorted to VBAT"},
  {SHORT_BUSPOS2GND, "Positive Wire Shorted to GND"},
  {SHORT_BUSNEG2VBAT, "Negative Wire Shorted to VBAT"},
  {SHORT_BUSPOS2BUSNEG, "Wires Shorted Together"},
  {CABLE_DISCONNECTED, "Wire Open or Wrong Port"},
  {CABLE_REVERSED, "Wires Interchanged"},
  {INTMSGERR, "Interrupt Messaging Error"}
};

// Function prototypes
int a2bWriteLocalReg(byte reg, byte data);
char a2bReadLocalReg(byte reg);
int a2bWriteLocalRegBlock(byte reg, byte len, byte data[]);
char* a2bReadLocalRegBlock(byte reg, byte len);
char a2bGetCurrentNodeAddr(void);
bool isPeriEnabled(void);
int a2bSetNodeAddr(int node, bool bcast, bool enablePeri);
int a2bWriteRemoteReg(int nodeAddr, byte reg, byte data);
char a2bReadRemoteReg(int nodeAddr, byte reg);
int a2bWriteRemoteRegBlock(int nodeAddr, byte reg, byte len, byte data[]);
char* a2bReadRemoteRegBlock(int nodeAddr, byte reg, byte len);
int a2bWriteRemotePeriReg(int nodeAddr, byte periAddr, byte reg, byte data);
int a2bWriteRemotePeriRegBlock(int nodeAddr, byte periAddr, byte reg, byte len, byte data[]);
char a2bReadRemotePeriReg(int nodeAddr, byte periAddr, byte reg);
char* a2bReadRemotePeriRegBlock(int nodeAddr, byte periAddr, byte reg, byte len);
a2bInt_t a2bReceiveInterrupt(void);

bool isPostDiscovery = false;

#endif
#ifndef __ADAU1452__
#define __ADAU1452__

#include "Settings.h"

// ADAU145x register definitions
typedef enum {
    // Phase locked loop control
    PLL_CTRL0 = 0xF000,
    PLL_CTRL1 = 0xF001,
    PLL_CLK_SRC = 0xF002,
    PLL_ENABLE = 0xF003,
    PLL_LOCK = 0xF004,
    MCLK_OUT = 0xF005,
    PLL_WATCHDOG = 0xF006,
    // Clock generators
    CLK_GEN1_M = 0xF020,
    CLK_GEN1_N = 0xF021,
    CLK_GEN2_M = 0xF022,
    CLK_GEN2_N = 0xF023,
    CLK_GEN3_M = 0xF024,
    CLK_GEN3_N = 0xF025,
    CLK_GEN3_SRC = 0xF026,
    CLK_GEN3_LOCK = 0xF027,
    // Power enable
    POWER_ENABLE0 = 0xF050,
    POWER_ENABLE1 = 0xF051,
    // ASRC I/O
    ASRC_INPUTn = 0xF100,
    ASRC_OUT_RATE = 0xF140,
    // Source of data for serial output ports
    SOUT_SOURCEn = 0xF180,
    // SPDIF transmitter data selector
    SPDIFTX_INPUT = 0xF1C0,
    // Serial port control
    SERIAL_BYTE_0_0 = 0xF200,
    SERIAL_BYTE_0_1 = 0xF201,
    SERIAL_BYTE_1_0 = 0xF204,
    SERIAL_BYTE_1_1 = 0xF205,
    SERIAL_BYTE_2_0 = 0xF208,
    SERIAL_BYTE_2_1 = 0xF209,
    SERIAL_BYTE_3_0 = 0xF20C,
    SERIAL_BYTE_3_1 = 0xF20D,
    SERIAL_BYTE_4_0 = 0xF210,
    SERIAL_BYTE_4_1 = 0xF211,
    SERIAL_BYTE_5_0 = 0xF214,
    SERIAL_BYTE_5_1 = 0xF215,
    SERIAL_BYTE_6_0 = 0xF218,
    SERIAL_BYTE_6_1 = 0xF219,
    SERIAL_BYTE_7_0 = 0xF21C,
    SERIAL_BYTE_7_1 = 0xF21D,
    // FTDM
    FTDM_INn = 0xF300,
    FTDM_OUTn = 0xF380,
    // DSP core control
    HIBERNATE = 0xF400,
    START_PULSE = 0xF401,
    START_CORE = 0xF402,
    KILL_CORE = 0xF403,
    START_ADDRESS = 0xF404,
    CORE_STATUS = 0xF405,
    // Panic manager
    PANIC_CLEAR = 0xF421,
    PANIC_PARITY_MASK = 0xF422,
    PANIC_SOFTWARE_MASK = 0xF423,
    PANIC_WD_MASK = 0xF424,
    PANIC_STACK_MASK = 0xF425,
    PANIC_LOOP_MASK = 0xF426,
    PANIC_FLAG = 0xF427,
    PANIC_CODE = 0xF428,
    EXECUTE_COUNT = 0xF432,
    SOFTWARE_VALUE_n = 0xF433,
    // Watchdog timer
    WATCHDOG_MAXCOUNT = 0xF443,
    WATCHDOG_PRESCALE = 0xF444,
    // Block interrupts
    BLOCKINT_EN = 0xF450,
    BLOCKINT_VALUE = 0xF451,
    // Program counter
    PROG_CNTR0 = 0xF460,
    PROG_CNTR1 = 0xF461,
    PROG_CNTR_CLEAR = 0xF462,
    PROG_CNTR_LENGTH0 = 0xF463,
    PROG_CNTR_LENGTH1 = 0xF464,
    PROG_CNTR_MAXLENGTH0 = 0xF465,
    PROG_CNTR_MAXLENGTH1 = 0xF466,
    // Multipurpose pins
    MPn_MODE = 0xF510,
    MPn_WRITE = 0xF520,
    MPn_READ = 0xF530,
    // DMIC
    DMIC_CTRL0 = 0xF560,
    DMIC_CTRL1 = 0xF561,
    // Audio sample rate conversion
    ASRC_LOCK = 0xF580,
    ASRC_MUTE = 0xF581,
    ASRCn_RATIO = 0xF582,
    ADC_READn = 0xF5A0,
    // SPDIF RX and TX controllers
    SPDIF_LOCK_DET = 0xF600,
    SPDIF_RX_CTRL = 0xF601,
    SPDIF_RX_DECODE = 0xF602,
    SPDIF_RX_COMPRMODE = 0xF603,
    SPDIF_RESTART = 0xF604,
    SPDIF_LOSS_OF_LOCK = 0xF605,
    SPDIF_AUX_EN = 0xF608,
    SPDIF_RX_AUXBIT_READY = 0xF60F,
    SPDIF_RX_CS_LEFT_n = 0xF610,
    SPDIF_RX_CS_RIGHT_n = 0xF620,
    SPDIF_RX_UD_LEFT_n = 0xF630,
    SPDIF_RX_UD_RIGHT_n = 0xF640,
    SPDIF_RX_VB_LEFT_n = 0xF650,
    SPDIF_RX_VB_RIGHT_n = 0XF660,
    SPDIF_RX_PB_LEFT_n = 0xF670,
    SPDIF_RX_PB_RIGHT_n = 0xF680,
    SPDIF_TX_EN = 0xF690,
    SPDIF_TX_CTRL = 0xF691,
    SPDIF_TX_AUXBIT_SOURCE = 0xF69F,
    SPDIF_TX_CS_LEFT_n = 0xF6A0,
    SPDIF_TX_CS_RIGHT_n = 0xF6B0,
    SPDIF_TX_UD_LEFT_n = 0xF6C0,
    SPDIF_TX_UD_RIGHT_n = 0xF6D0,
    SPDIF_TX_VB_LEFT_n = 0xF6E0,
    SPDIF_TX_VB_RIGHT_n = 0XF6F0,
    SPDIF_TX_PB_LEFT_n = 0xF700,
    SPDIF_TX_PB_RIGHT_n = 0xF710,
    // Pin drive strength and slew rate
    BCLK_IN0_PIN = 0xF780,
    BCLK_IN1_PIN = 0xF781,
    BCLK_IN2_PIN = 0xF782,
    BCLK_IN3_PIN = 0xF783,
    BCLK_OUT0_PIN = 0xF784,
    BCLK_OUT1_PIN = 0xF785,
    BCLK_OUT2_PIN = 0xF786,
    BCLK_OUT3_PIN = 0xF787,
    LRCLK_IN0_PIN = 0xF788,
    LRCLK_IN1_PIN = 0xF789,
    LRCLK_IN2_PIN = 0xF78A,
    LRCLK_IN3_PIN = 0xF78B,
    LRCLK_OUT0_PIN = 0xF78C,
    LRCLK_OUT1_PIN = 0xF78D,
    LRCLK_OUT2_PIN = 0xF78E,
    LRCLK_OUT3_PIN = 0xF78F,
    SDATA_IN0_PIN = 0xF790,
    SDATA_IN1_PIN = 0xF791,
    SDATA_IN2_PIN = 0xF792,
    SDATA_IN3_PIN = 0xF793,
    SDATA_OUT0_PIN = 0xF794,
    SDATA_OUT1_PIN = 0xF795,
    SDATA_OUT2_PIN = 0xF796,
    SDATA_OUT3_PIN = 0xF797,
    SPDIF_TX_PIN = 0xF798,
    SCLK_SCL_PIN = 0xF799,
    MISO_SDA_PIN = 0xF79A,
    SS_PIN = 0xF79B,
    MOSI_ADDR1_PIN = 0xF79C,
    SCLK_SCL_M_PIN = 0xF79D,
    MISO_SDA_M_PIN = 0xF79E,
    SS_M_PIN = 0xF79F,
    MOSI_M_PIN = 0xF7A0,
    MP6_PIN = 0xF7A1,
    MP7_PIN = 0xF7A2,
    CLKOUT_PIN = 0xF7A3,
    SOFT_RESET = 0xF890
} ADAU145xRegs;

// Hardware definitions for ADAU145x
#define MAX_NUM_ASRC 8
#define MAX_NUM_SOUT_CHANNELS 48
#define MAX_NUM_FTDM 64
#define MAX_NUM_MP_PINS 14
#define MAX_NUM_ADC 6
#define MAX_NUM_SPDIF_CHANNELS 12

// Data memory and program memory regions
#define DM0_BASE 0x0000
#define DM1_BASE 0x6000
#define PROGMEM_BASE 0xC000

#define PROGMEM_SIZE 0x2000

#if DSP_CORE == SIGMA100
#define DM0_SIZE 0x5000
#define DM1_SIZE 0x5000
#elif DSP_CORE == SIGMA200
#define DM0_SIZE 0x4000
#define DM1_SIZE 0x4000
#elif DSP_CORE == SIGMA300
#define DM0_SIZE 0x2000
#define DM1_SIZE 0x2000
#endif

// Safeload regions
#define REG_SAFELOAD_BASE DM1_BASE
#define REG_SAFELOAD_TARGET_ADDR (REG_SAFELOAD_BASE + 5)
#define REG_SAFELOAD_NUM (REG_SAFELOAD_BASE + 6)

#define MAX_NUM_SAFELOAD_DATASLOTS 5

void dspSoftReset(bool state);
void dspHibernate(bool state);
void dspStartCore(bool state);
void dspKillCore(bool state);
int dspInit(void);

bool dspInitDone = false;

#endif
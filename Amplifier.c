#include <Arduino.h>
#include "Settings.h"
#include "A2B.h"
#include "Amplifier.h"
#include "Utils.h"

/// @brief Sets the initial register states of the GPIO expander
/// @param  
/// @return 1 on successful execution
int pcaInit(void) {
    int retval = 0;
    retval += a2bWriteRemotePeriReg(ampNodeID, I2CADDR_PCA9538, PCA9538_OUT_PORT, 0x00);
    retval += a2bWriteRemotePeriReg(ampNodeID, I2CADDR_PCA9538, PCA9538_CFG, 0x00);

    return retval == 2 ? 1 : -1;
}

int pcaWriteReg(byte reg, byte mask) {
  int result = a2bWriteRemotePeriReg(ampNodeID, I2CADDR_PCA9538, reg, mask);
  if (result != RESULT_SUCCESS) {
    Serial.print("Error writing GPIO expander register: ");
    Serial.println(reg, HEX);
  }
  return result;
}

byte pcaReadReg(byte reg) {
  return a2bReadRemotePeriReg(ampNodeID, I2CADDR_PCA9538, reg);
}

/// @brief Sets the output state of a GPIO expander pin
/// @param gpio 
/// @param state 
/// @return 1 on successful execution
int pcaWritePin(int gpio, bool state) {
    if (gpio < PCA9538_MAX_GPIOS) {
        unsigned char currentState = (unsigned char)a2bReadRemotePeriReg(ampNodeID, I2CADDR_PCA9538, PCA9538_OUT_PORT);
        
        if (state) {
            currentState |= (1 << gpio);
        } else {
            currentState &= ~(1 << gpio);
        }

        return a2bWriteRemotePeriReg(
            ampNodeID, 
            I2CADDR_PCA9538,
            PCA9538_OUT_PORT,
            currentState
        ); 
    }
    return -1;
}

/// @brief Reads the current input state of a GPIO expander pin
/// @param gpio 
/// @return boolean value, 0=low, 1=high
bool pcaReadPin(int gpio) {
    if (gpio >= 0 && gpio < PCA9538_MAX_GPIOS) {
        int mask = 1 << gpio;

        return (a2bReadRemotePeriReg(ampNodeID, I2CADDR_PCA9538, PCA9538_IN_PORT) & mask) == mask;
    }
    Serial.println("Invalid GPIO index");
    return false;
}

/// @brief Sets a GPIO expander pin to be either an input or output
/// @param gpio 
/// @param direction 
/// @return 1 on successful execution
int pcaSetPinDirection(int gpio, bool direction) {
    if (gpio < PCA9538_MAX_GPIOS) {
        unsigned char currentState = (unsigned char)a2bReadRemotePeriReg(ampNodeID, I2CADDR_PCA9538, PCA9538_CFG);

        if (direction) {
            currentState |= (1 << gpio);
        } else {
            currentState &= ~(1 << gpio);
        }

        return a2bWriteRemotePeriReg(
            ampNodeID, 
            I2CADDR_PCA9538,
            PCA9538_CFG,
            currentState
        ); 
    }
    return -1;
}

/// @brief Sets the polarity of a GPIO expander pin
/// @param gpio 
/// @param polarity 
/// @return 1 on successful execution
int pcaInvertPinPolarity(int gpio, bool polarity) {
    if (gpio < PCA9538_MAX_GPIOS) {
        unsigned char currentState = (unsigned char)a2bReadRemotePeriReg(ampNodeID, I2CADDR_PCA9538, PCA9538_INV);

        if (polarity) {
            currentState |= (1 << gpio);
        } else {
            currentState &= ~(1 << gpio);
        }

        return a2bWriteRemotePeriReg(
            ampNodeID, 
            I2CADDR_PCA9538,
            PCA9538_INV,
            currentState
        ); 
    }
    return -1;
}

/// @brief Returns the 8-byte amplifier identification data stored in it's EEPROM
/// @param  
/// @return 8 byte char array
char* getAmplifierID(void) {
    char* data = a2bReadRemotePeriRegBlock(ampNodeID, I2CADDR_EEPROM_AMP, 0x00, 8);

    return data;
}

/// @brief Initialization sequence for the Tesla premium amplifier
/// @param  
/// @return 1 on successful execution
int amplifierInit(void) {
    int retval = 0;

    retval += pcaInit();

    retval += pcaWritePin(PIN_TDA7802_HWMUTE_N, HIGH);
    retval += pcaWritePin(PIN_AMP_STATUS_LED, HIGH);
    retval += pcaSetPinDirection(PIN_BOOST_OVERTEMP, INPUT);
    retval += pcaSetPinDirection(PIN_AMPS_OVERTEMP, INPUT);

    delayMicroseconds(30000);

    retval += pcaWritePin(PIN_TDA7802_EN, HIGH);

    delayMicroseconds(30000);

    retval += a2bWriteRemotePeriRegBlock(ampNodeID, I2CADDR_TDA7802, 0x00, sizeof(tda7802ConfigData), tda7802ConfigData);
    retval += a2bWriteRemotePeriRegBlock(ampNodeID, I2CADDR_FDA2100, 0x00, sizeof(fda2100ConfigData), fda2100ConfigData);

    return retval == 8 ? 1 : -1;
}
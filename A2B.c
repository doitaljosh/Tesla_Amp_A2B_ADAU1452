#include "A2B.h"

/// @brief Write to a master register
/// @param reg 
/// @param len 
/// @param data 
/// @return 1 on successful write
int a2bWriteLocalReg(byte reg, byte data) {
    Wire.beginTransmission(I2CADDR_A2B_MASTER);
    Wire.write(reg);
    Wire.write(data, 1);
    Wire.endTransmission();

    return 1;
}

int a2bWriteLocalRegBlock(byte reg, byte len, byte data[]) {
    if (len < I2C_MAX_DATA_LEN) {
        Wire.beginTransmission(I2CADDR_A2B_MASTER);
        Wire.write(reg);
        Wire.write(data, len);
        Wire.endTransmission();

        return 1;
    }
    return 0;
}

/// @brief Read from a master register
/// @param i2cAddr 
/// @param reg 
/// @param len 
/// @return an array of register data of length=len
char a2bReadLocalReg(byte reg) {
    Wire.beginTransmission(I2CADDR_A2B_MASTER);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(I2CADDR_A2B_MASTER, len);
        
    char regval = 0x00;
    
    Wire.readBytes(regval, 1);

    return regval;
}

char* a2bReadLocalRegBlock(byte reg, byte len) {
    if (len < I2C_MAX_DATA_LEN) {
        Wire.beginTransmission(I2CADDR_A2B_MASTER);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(I2CADDR_A2B_MASTER, len);
        
        char* regval = (char*)malloc(len);
        if (regval != NULL) {
            if (Wire.readBytes(regval, len) != len) {
                free(regval);
                return NULL;
            }
            return regval;
        } else {
            // Allocation failed
            return NULL;
        }
    }
    return NULL;
}

/// @brief Get the current node address set in the master node address register
/// @param  
/// @return a 5 bit value of the current node address
char a2bGetCurrentNodeAddr(void) {
    return (a2bReadLocalReg(AD242x_REG_NODEADR) & 0x1f);
}

/// @brief Checks if the peripheral bit in A2B_NODEADR is set to 1
/// @param  
/// @return boolean value of the A2B_NODEADR.PERI bit
bool isPeriEnabled(void) {
  if ((a2bReadLocalReg(AD242x_REG_NODEADR) & 0x20) == 1) {
    return true;
  } else {
    return false;
  }
}

/// @brief Sets a new node address and peripheral mode if they aren't already the same
/// @param node 
/// @param bcast 
/// @param enablePeri 
/// @return 1 on successful execution
int a2bSetNodeAddr(int node, bool bcast, bool enablePeri) {
    if (((node & 0x1f) != a2bGetCurrentNodeAddr()) && (enablePeri != isPeriEnabled())) {
        if (!(node < 0 || node > 31)) {
            unsigned char regval = node & 0x1F;

            if (bcast) { regval |= (1 << 7); }
            if (enablePeri) { regval |= (1 << 5); }
            
            Wire.beginTransmission(I2CADDR_A2B_MASTER);
            Wire.write(AD242x_REG_NODEADR);
            Wire.write(regval);
            
            return (Wire.endTransmission() == 0) ? 1 : 0;
        }
        return 0;
    }
    return 0;
}

/// @brief Writes to a register on a remote A2B node
/// @param nodeAddr 
/// @param reg 
/// @param len 
/// @param data 
/// @return 1 on successful execution
int a2bWriteRemoteReg(int nodeAddr, byte reg, byte data) {
    if ((nodeAddr & 0xe0) == 0) {
        a2bSetNodeAddr(nodeAddr, 0, 0);

        Wire.beginTransmission(I2CADDR_A2B_SLAVE);
        Wire.write(reg);
        Wire.write(data);
        Wire.endTransmission();
        return 1;
    }
    return 0;
}

int a2bWriteRemoteRegBlock(int nodeAddr, byte reg, byte len, byte data[]) {
    if ((nodeAddr & 0xe0) == 0) {
        a2bSetNodeAddr(nodeAddr, 0, 0);

        if (len < I2C_MAX_DATA_LEN) {
            Wire.beginTransmission(I2CADDR_A2B_SLAVE);
            Wire.write(reg);
            for (int i = 0; i < len; i++) {
                Wire.write(data[i]);
            }
            Wire.endTransmission();
            return 1;
        }
        return 0;
    }
    return 0;
}

/// @brief Reads from a register on a remote A2B node
/// @param nodeAddr 
/// @param reg 
/// @param len 
/// @return an array of length len with the register values starting from reg
char a2bReadRemoteReg(int nodeAddr, byte reg) {
    if ((nodeAddr & 0xe0) == 0) {
        a2bSetNodeAddr(nodeAddr, 0, 0);

        Wire.beginTransmission(I2CADDR_A2B_SLAVE);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(I2CADDR_A2B_SLAVE, 1);

        char regval = 0x00;

        Wire.readBytes(regval, 1);

        return regval;
    }
    return NULL;
}

char* a2bReadRemoteRegBlock(int nodeAddr, byte reg, byte len) {
    if ((nodeAddr & 0xe0) == 0) {
        a2bSetNodeAddr(nodeAddr, 0, 0);

        Wire.beginTransmission(I2CADDR_A2B_SLAVE);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(I2CADDR_A2B_SLAVE, len);

        char* regval = (char*)malloc(len);
        if (regval != NULL) {
            if (Wire.readBytes(regval, len) != len) {
                free(regval);
                return NULL;
            }
            return regval;
        } else {
            // Allocation failed
            return NULL;
        }
    }
    return NULL;
}

/// @brief Writes to registers of an I2C peripheral on a remote A2B node
/// @param nodeAddr 
/// @param periAddr 
/// @param reg 
/// @param len 
/// @param data 
/// @return 1 on successful execution
int a2bWriteRemotePeriReg(int nodeAddr, byte periAddr, byte reg, byte data) {
    if ((nodeAddr & 0xe0) == 0) {
        a2bSetNodeAddr(nodeAddr, 0, 0);
        a2bWriteRemoteReg(nodeAddr, AD242x_REG_CHIP, periAddr);
        a2bSetNodeAddr(nodeAddr, 0, 1);
        return a2bWriteRemoteReg(nodeAddr, reg, data);
    }
    return 0;
}

int a2bWriteRemotePeriRegBlock(int nodeAddr, byte periAddr, byte reg, byte len, byte data[]) {
    if ((nodeAddr & 0xe0) == 0) {
        a2bSetNodeAddr(nodeAddr, 0, 0);
        a2bWriteRemoteRegBlock(nodeAddr, AD242x_REG_CHIP, 1, periAddr);
        a2bSetNodeAddr(nodeAddr, 0, 1);
        return a2bWriteRemoteRegBlock(nodeAddr, reg, len, data);
    }
    return 0;
}

/// @brief Reads from registers of an I2C peripheral on a remote A2B node
/// @param nodeAddr 
/// @param periAddr 
/// @param reg 
/// @param len 
/// @return 
char a2bReadRemotePeriReg(int nodeAddr, byte periAddr, byte reg) {
    if ((nodeAddr & 0xe0) == 0) {
        a2bSetNodeAddr(nodeAddr, 0, 0);
        a2bWriteRemoteReg(nodeAddr, AD242x_REG_CHIP, periAddr);
        a2bSetNodeAddr(nodeAddr, 0, 1);
        return a2bReadRemoteReg(nodeAddr, reg);
    }
    return NULL;
}

char* a2bReadRemotePeriRegBlock(int nodeAddr, byte periAddr, byte reg, byte len) {
    if ((nodeAddr & 0xe0) == 0) {
        a2bSetNodeAddr(nodeAddr, 0, 0);
        a2bWriteRemoteRegBlock(nodeAddr, AD242x_REG_CHIP, 1, periAddr);
        a2bSetNodeAddr(nodeAddr, 0, 1);
        return a2bReadRemoteRegBlock(nodeAddr, reg, len);
    }
    return NULL;
}

a2bInt_t a2bReceiveInterrupt(void) {
  byte intType = a2bReadLocalReg(AD242x_REG_INTTYPE);
  byte intSrc = a2bReadLocalReg(AD242x_REG_INTSRC);

  a2bInt_t a2bint;

  if (intType << 6) {
    a2bint.MstrSub = A2B_INTSRC_SUB;
  } else {
    a2bint.MstrSub = A2B_INTSRC_MASTER;
  }

  a2bint.nodeId = (int)(intSrc & 0x0F);
  a2bint.intType = (int)intType;

  return a2bint;
}